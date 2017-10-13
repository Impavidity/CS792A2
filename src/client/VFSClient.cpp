//
// Created by shipeng on 17-10-13.
//

#include "VFSClient.h"

RPCClient VFSClient::rpcClient;

int VFSClient::nfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
  int fh;
  rpcClient.nfs_create(path, mode, fi);
}

int VFSClient::nfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi,
                           enum fuse_readdir_flags flags) {
  int ret;
  ret = rpcClient.nfs_readdir(path, buf, filler, offset, fi, flags);
  return ret;
}

int VFSClient::nfs_mkdir(const char *path, mode_t mode) {
  int ret;
  std::cout << "In Mkdir " << path <<std::endl;
  ret = rpcClient.nfs_mkdir(path, mode);
  return ret;
}

int VFSClient::nfs_rmdir(const char *path) {
  int ret;
  std::cout << "In RMDIR" << path << std::endl;
  ret = rpcClient.nfs_rmdir(path);
  return ret;
}

int VFSClient::nfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
  int ret;
  // Use vnode the process the path component by component
  ret = rpcClient.nfs_getattr(path, stbuf, fi);
  return ret;
}

void* VFSClient::nfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
  (void) conn;
  cfg->kernel_cache = 1;
  return NULL;
}

