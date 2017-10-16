//
// Created by shipeng on 17-10-13.
//

#include "VFSClient.h"

RPCClient VFSClient::rpcClient;

VNodeClient VFSClient::root(&rpcClient);
CacheClient VFSClient::cache;


int VFSClient::mount(char* rootPath) {
  thrift_file_handler fh;
  fh = rpcClient.mount(rootPath);
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
  VNodeClient* cache_vnode = nullptr;//= cache.checkPath(tpath);
  if (cache_vnode== nullptr || cache_vnode->getattr_state == 0) {
    VNodeClient vnode = VNodeClient::getattr(&root, tpath);
    vnode.getattr_state = 1;
    cache.insertPath(tpath, vnode);
    cache_vnode = &vnode;
  }
  if (cache_vnode->getattr_reply.ret > -1) {
    thrift2stat(cache_vnode->getattr_reply.tstbuf, stbuf);
  }
  std::cout << "get attr reply" << cache_vnode->getattr_reply.ret << std::endl;
  return cache_vnode->getattr_reply.ret;

  //ret = rpcClient.nfs_getattr(path, stbuf, fi);
  //return ret;
}

int VFSClient::nfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi,
                           enum fuse_readdir_flags flags) {
  std::string tpath(path);
  std::cout << "in readdir " << tpath << std::endl;
  VNodeClient* cache_vnode = nullptr;//= cache.checkPath(path);
  if (cache_vnode== nullptr || cache_vnode->readdir_state ==0) {
    VNodeClient vnode = VNodeClient::readdir(&root, tpath);
    vnode.readdir_state = 1;
    cache.insertPath(tpath, vnode);
    cache_vnode = &vnode;
  }
  for (thrift_dir_entry tde : cache_vnode->readdir_reply.dir_entries) {
    filler(buf, tde.dir_name.c_str(), NULL, 0, FUSE_FILL_DIR_PLUS);
  }
  return cache_vnode->readdir_reply.ret;

//  ret = rpcClient.nfs_readdir(path, buf, filler, offset, fi, flags);
//  return ret;
//  for (thrift_dir_entry tde : reply.dir_entries) {
//    filler(buf, tde.dir_name.c_str(), NULL, 0, FUSE_FILL_DIR_PLUS);
//  }
//
//  return reply.ret;
}


//int VFSClient::nfs_mkdir(const char *path, mode_t mode) {
//  int ret;
//  std::string tpath(path);
//
//  std::cout << "In Mkdir " << path <<std::endl;
//  ret = rpcClient.nfs_mkdir(path, mode);
//  return ret;
//}
//
//int VFSClient::nfs_rmdir(const char *path) {
//  int ret;
//  std::cout << "In RMDIR" << path << std::endl;
//  ret = rpcClient.nfs_rmdir(path);
//  return ret;
//}
//
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

