//
// Created by shipeng on 17-10-13.
//

#include "VFSClient.h"

RPCClient VFSClient::rpcClient;

VNodeClient VFSClient::root(&rpcClient);
CacheClient VFSClient::cache;


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
  std::string tpath(path);
  std::cout << "in getattr " << tpath << std::endl;
  std::cout << "-------------Cache-Getattr-------------" << std::endl;
  for (auto pair: cache.path2vnode) std::cout << pair.first << " ---- " << pair.second.getattr_state << " ---- " << pair.second.getattr_reply << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  if (cache_vnode != nullptr && cache_vnode->getattr_state ==0) {
    std::cout << "Find cache, but no getattr info" << std::endl;
    vnode = VNodeClient::getattr(&root, tpath);
    cache_vnode->getattr_reply = vnode.getattr_reply;
    cache_vnode->getattr_state = 1;
  } else if (cache_vnode== nullptr) {
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
  std::cout << "in readdir " << tpath << std::endl;
  std::cout << "-------------Cache-Readdir-------------" << std::endl;
  for (auto pair: cache.path2vnode) std::cout << pair.first << " ---- " << pair.second.readdir_state << " ---- " << pair.second.readdir_reply << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(path);
  std::cout << "checking path " << path << std::endl;
  VNodeClient vnode;
  if (cache_vnode != nullptr && cache_vnode->readdir_state ==0) {
    std::cout << "Find cache, but no readdir info" << std::endl;
    vnode = VNodeClient::readdir(&root, tpath); //cache_vnode, std::string("")
    cache_vnode->readdir_reply = vnode.readdir_reply;
    cache_vnode->readdir_state = 1;
  } else if (cache_vnode == nullptr) {
    std::cout << "Find cache failed" << std::endl;
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
  std::cout  << "****************mkdir*****************" << std::endl;
  std::cout << "path for mkdir " << tpath << std::endl;
  auto index = tpath.rfind('/');
  std::string first_part, second_part;
  if (index !=std::string::npos)  {
    first_part = tpath.substr(0, index);
    second_part = tpath.substr(index+1);
    std::cout << "Split path in mkdir " << first_part << " " << second_part << std::endl;
  }
  std::cout << "in making dir " << tpath << std::endl;
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
  std::cout << "In RMDIR" << path << std::endl;
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

//int VFSClient::nfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
//  int fh;
//  rpcClient.nfs_create(path, mode, fi);
//  return 0;
//}



void* VFSClient::nfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
  (void) conn;
  cfg->kernel_cache = 1;
  return NULL;
}

int VFSClient::nfs_open(const char *path, struct fuse_file_info *fi) {
  std::string tpath(path);
  std::cout << "VFSClient::nfs_open : path " << tpath << std::endl;
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
  std::cout << "VFSClient::nfs_read : path " << tpath << std::endl;
  VNodeClient* cache_vnode = cache.checkPath(tpath);
  VNodeClient vnode;
  if (cache_vnode != nullptr && cache_vnode->read_state==0) {
    std::cout << "Find cache, but no read content info" << std::endl;
    vnode = VNodeClient::read(&root, tpath, size, offset); //cache_vnode, std::string("")
    cache_vnode->read_reply = vnode.read_reply;
    cache_vnode->read_state = 1;
  } else if (cache_vnode == nullptr) {
    std::cout << "Find cache failed" << std::endl;
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

