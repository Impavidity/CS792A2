//
// Created by shipeng on 17-10-13.
//

#include "VFSClient.h"

RPCClient VFSClient::rpcClient;

VNodeClient VFSClient::root(&rpcClient);
CacheClient VFSClient::cache;

static bool checkWriteSync(std::vector<WriteRecord>* pt, int64_t verifier) {
  for (int i=0; i<pt->size(); i++) {
    if ((*pt)[i].write_verifier != verifier)
      return false;
  }
  return true;
}


int VFSClient::mount(char* rootPath) {
  thrift_file_handler fh;
  fh = rpcClient.mount(rootPath).fh;
  if (fh.inode == 0) {
    std::cout << "Dir does not exist" << std::endl;
    exit(1);
  } else {
    root.fh = fh;
  }
  return 0;
}

int VFSClient::getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
  // Use vnode the process the path component by component
  // No cache for getattr reply(just saved but needs updated before every new action)
  // But check file handler before getattr.
  // We always need to get mtime to check
  std::string tpath(path);
  //std::cout << "in getattr " << tpath << std::endl;
  //std::cout << "-------------Cache-Getattr-------------" << std::endl;
  //for (auto pair: cache.path2vnode) std::cout << pair.first << " ---- " << pair.second.getattr_state << " ---- " << pair.second.getattr_reply << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  // Cache Consistence here
  // If timeout, set all state to zero (readdir, read)
  // Another problem is that we cache the vnode and filehandler.
  // If cannot find filehandler in lookup, just try lookup from root with fullpath again and updated the vnode
  if (cache_vnode != nullptr) {
    //std::cout << "Find cache, but no getattr info" << std::endl;
    vnode = VNodeClient::getattr(&root, tpath, cache_vnode);
    cache_vnode->getattr_reply = vnode.getattr_reply;
    cache_vnode->getattr_state = 1;
  } else {
    vnode = VNodeClient::getattr(&root, tpath);
    if (vnode.getattr_reply.ret > -1) {
      vnode.getattr_state = 1;
      cache.insertPath(tpath, vnode);
    }
    cache_vnode = &vnode;
  }
  if (cache_vnode->getattr_reply.ret > -1) {
    thrift2stat(cache_vnode->getattr_reply.tstbuf, stbuf);
  }
  //std::cout << cache_vnode->getattr_reply << std::endl;
  //std::cout << "get attr reply " << "return value " << cache_vnode->getattr_reply.ret << " inode number "  << cache_vnode->fh.inode << std::endl;
  return cache_vnode->getattr_reply.ret;

  //ret = rpcClient.nfs_getattr(path, stbuf, fi);
  //return ret;
}

int VFSClient::nfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi,
                           enum fuse_readdir_flags flags) {
  std::string tpath(path);
  //std::cout << "in readdir " << tpath << std::endl;
  //std::cout << "-------------Cache-Readdir-------------" << std::endl;
  //for (auto pair: cache.path2vnode) std::cout << pair.first << " ---- " << pair.second.readdir_state << " ---- " << pair.second.readdir_reply << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(path);
  //std::cout << "checking path " << path << std::endl;
  VNodeClient vnode;
  if (cache_vnode != nullptr && cache_vnode->readdir_state==1) {
    vnode = VNodeClient::readdir(&root, tpath, cache_vnode);
    cache_vnode = &vnode;
  }
  if (cache_vnode != nullptr && cache_vnode->readdir_state==0) {
    //std::cout << "Find cache, but no readdir info" << std::endl;
    //vnode = VNodeClient::readdir(&root, tpath); //cache_vnode, std::string("")
    vnode = VNodeClient::readdir(cache_vnode, std::string(""));
    cache_vnode->readdir_reply = vnode.readdir_reply;
    cache_vnode->readdir_state = 1;
  } else if (cache_vnode == nullptr) {
    //std::cout << "Find cache failed" << std::endl;
    vnode = VNodeClient::readdir(&root, tpath);
    vnode.readdir_state = 1;
    cache.insertPath(tpath, vnode);
    cache_vnode = &vnode;
  }
  for (thrift_dir_entry tde : cache_vnode->readdir_reply.dir_entries) {
    filler(buf, tde.dir_name.c_str(), NULL, 0, FUSE_FILL_DIR_PLUS);
  }
  return cache_vnode->readdir_reply.ret;
}


int VFSClient::nfs_mkdir(const char *path, mode_t mode) {
  std::string tpath(path);
  //std::cout  << "****************mkdir*****************" << std::endl;
  //std::cout << "path for mkdir " << tpath << std::endl;
  auto index = tpath.rfind('/');
  std::string first_part, second_part;
  if (index !=std::string::npos)  {
    first_part = tpath.substr(0, index);
    second_part = tpath.substr(index+1);
    //std::cout << "Split path in mkdir " << first_part << " " << second_part << std::endl;
  }
  //std::cout << "in making dir " << tpath << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(first_part);
  VNodeClient vnode;
  if (cache_vnode != nullptr) {
    vnode = VNodeClient::mkdir(cache_vnode, std::string(""), second_part);
  } else {
    vnode = VNodeClient::mkdir(&root, first_part, second_part);
  }
  // TODO : cache
  return vnode.mkdir_reply.ret;
}

int VFSClient::nfs_rmdir(const char *path) {
  std::string tpath(path);
  //std::cout << "In RMDIR" << path << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  if (cache_vnode != nullptr) {
    vnode = VNodeClient::rmdir(cache_vnode, std::string(""));
  } else {
    vnode = VNodeClient::rmdir(&root, tpath);
  }
  // TODO: refine cache
  if (vnode.rmdir_reply > -1) {
    cache.removePath(tpath);
  };
  return vnode.rmdir_reply;
}


void* VFSClient::nfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
  (void) conn;
  cfg->kernel_cache = 1;
  return NULL;
}

int VFSClient::nfs_open(const char *path, struct fuse_file_info *fi) {
  std::string tpath(path);
  //std::cout << "VFSClient::nfs_open : path " << tpath << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  // TODO: cache
  // if (cache_vnode != nullptr)
  if (cache_vnode == nullptr) {
    vnode = VNodeClient::open(&root, tpath);
  }
  return vnode.open_state;
}

int VFSClient::nfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
  std::string tpath(path);
  //std::cout << "VFSClient::nfs_read : path " << tpath << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  if (cache_vnode != nullptr && cache_vnode->read_state==0) {
    //std::cout << "Find cache, but no read content info" << std::endl;
    vnode = VNodeClient::read(&root, tpath, size, offset); //cache_vnode, std::string("")
    cache_vnode->read_reply = vnode.read_reply;
    cache_vnode->read_state = 1;
  } else if (cache_vnode == nullptr) {
    //std::cout << "Find cache failed" << std::endl;
    vnode = VNodeClient::read(&root, tpath, size, offset);
    vnode.read_state = 1;
    cache.insertPath(tpath, vnode);
    cache_vnode = &vnode;
  }
  if (cache_vnode->read_reply.ret > -1) {
    int64_t len = cache_vnode->read_reply.buf.size();
    if (offset < len) {
      if (offset + size > len)
        size = (size_t) (len - offset);
      memcpy(buf, cache_vnode->read_reply.buf.c_str(), size);
    } else {
      size = 0;
    }
  } else {
    size = 0;
  }
  return (int) size;
}

int VFSClient::nfs_write(const char *path, const char *buf, size_t size, off_t offset, fuse_file_info *fi) {
  std::string tpath(path);
  std::cout << "VFSClient::nfs_write : path " << tpath << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  if (cache_vnode != nullptr) {
    //TODO : Cache
  } else {
    //std::cout << "Find cache failed" << std::endl;
    vnode = VNodeClient::write(&root, tpath, buf, size, offset);
    cache.insertWriteRecord(vnode.fh.inode, WriteRecord(tpath, std::string(buf), size, offset, vnode.write_reply.write_verifier, *fi));
    cache.insertPath(tpath, vnode);
    cache_vnode = &vnode;
  }
  return (int) cache_vnode->write_reply.ret;
}

int VFSClient::nfs_release(const char* path, struct fuse_file_info* fi) {
  std::string tpath(path);
  std::cout << "VFSClient::nfs_release : path " << tpath << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  if (cache_vnode != nullptr) {
    //TODO : Cache
  } else {
    //std::cout << "Find cache failed" << std::endl;
    //TODO : Cache
    vnode = VNodeClient::fsync(&root, tpath);
    std::vector<WriteRecord>* pt = cache.getWriteVectorPT(vnode.fh.inode);
    if (pt == nullptr) return 0;
    while (true) {
      if (! checkWriteSync(pt, vnode.fsync_reply.write_verifier)) {
        for (int i=0; i<pt->size(); i++) {
          VNodeClient temp = VNodeClient::write(&root, (*pt)[i].path, (*pt)[i].content.c_str(), (*pt)[i].size, (*pt)[i].offset);
          (*pt)[i].write_verifier = temp.write_reply.write_verifier;
        }
        vnode = VNodeClient::fsync(&root, tpath);
      } else {
        cache.flushWriteRecord(vnode.fh.inode);
        break;
      }
    }
    cache_vnode = &vnode;
  }

  return 0;
}

int VFSClient::nfs_fsync(const char* path, int datasync, struct fuse_file_info* fi) {
  std::string tpath(path);
  std::cout << "VFSClient::nfs_release datasync " <<  datasync << " : path " << tpath << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  if (cache_vnode != nullptr) {
    //TODO: Cache
  } else {
    //std::cout << "Find cache failed" << std::endl;
    //TODO : Cache
    vnode = VNodeClient::fsync(&root, tpath);
    std::vector<WriteRecord>* pt = cache.getWriteVectorPT(vnode.fh.inode);
    if (pt == nullptr) return 0;
    while (true) {
      if (! checkWriteSync(pt, vnode.fsync_reply.write_verifier)) {
        for (int i=0; i<pt->size(); i++) {
          VNodeClient temp = VNodeClient::write(&root, (*pt)[i].path, (*pt)[i].content.c_str(), (*pt)[i].size, (*pt)[i].offset);
          (*pt)[i].write_verifier = temp.write_reply.write_verifier;
        }
        vnode = VNodeClient::fsync(&root, tpath);
      } else {
        cache.flushWriteRecord(vnode.fh.inode);
        break;
      }
    }
    cache_vnode = &vnode;
  }

  return 0;
}

int VFSClient::nfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
  std::string tpath(path);
  std::cout  << "VFSClient::nfs_create path " << tpath << std::endl;
  auto index = tpath.rfind('/');
  std::string first_part, second_part;
  if (index !=std::string::npos)  {
    first_part = tpath.substr(0, index);
    second_part = tpath.substr(index+1);
    //std::cout << "Split path in create file " << first_part << " " << second_part << std::endl;
  }
  VNodeClient* cache_vnode = cache.checkPath(first_part);
  VNodeClient vnode;
  if (cache_vnode != nullptr) {
    vnode = VNodeClient::create(cache_vnode, std::string(""), second_part);
  } else {
    vnode = VNodeClient::create(&root, first_part, second_part);
  }
  return vnode.create_reply.ret;
}

int VFSClient::nfs_unlink(const char *path) {
  std::string tpath(path);
  std::cout << "VFSClient::nfs_unlink " << tpath << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  if (cache_vnode != nullptr) {
    vnode = VNodeClient::unlink(cache_vnode, std::string(""));
  } else {
    vnode = VNodeClient::unlink(&root, tpath);
  }
  return vnode.unlink_reply;
}
