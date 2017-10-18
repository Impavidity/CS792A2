//
// Created by shipeng on 17-10-13.
//

#ifndef NFS_FUSE_VFSCLIENT_H
#define NFS_FUSE_VFSCLIENT_H

#include "RPCClient.h"
#include "VNodeClient.h"
#include "CacheClient.h"

class VFSClient {
public:
  int mode = 0; // 0: NFS
  static RPCClient rpcClient;
  static VNodeClient root;
  static CacheClient cache;
  VFSClient() {}
  static int mount(char* rootPath);
  static int getattr(const char* path, struct stat* stbuf, struct fuse_file_info *fi);
  static int nfs_readdir(const char* path, void * buf, fuse_fill_dir_t filler, off_t offset,
                         struct fuse_file_info *fi, enum fuse_readdir_flags flags);
  static int nfs_mkdir(const char* path, mode_t mode);
  static int nfs_rmdir(const char* path);
  static int nfs_open(const char *path, struct fuse_file_info *fi);
  static int nfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
  static int nfs_write(const char *path, const char *buf, size_t size, off_t offset, fuse_file_info *fi);
  static int nfs_release(const char *path, struct fuse_file_info* fi);
  static int nfs_fsync(const char* path, int, fuse_file_info* fi);
  static int nfs_create(const char *path, mode_t mode, struct fuse_file_info* fi);
  static void *nfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg);
  static int nfs_unlink(const char * path);

};


#endif //NFS_FUSE_VFSCLIENT_H
