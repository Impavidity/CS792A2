//
// Created by shipeng on 17-10-13.
//

#ifndef NFS_FUSE_VFSCLIENT_H
#define NFS_FUSE_VFSCLIENT_H

#include "RPCClient.h"

class VFSClient {
public:
  int mode = 0; // 0: NFS
  static RPCClient rpcClient;

  VFSClient() {}
  static int nfs_create(const char *path, mode_t mode, struct fuse_file_info* fi);
  static int nfs_readdir(const char* path, void * buf, fuse_fill_dir_t filler, off_t offset,
                         struct fuse_file_info *fi, enum fuse_readdir_flags flags);
  static int nfs_mkdir(const char* path, mode_t mode);
  static int nfs_rmdir(const char* path);
  static int nfs_getattr(const char* path, struct stat* stbuf, struct fuse_file_info *fi);
  static void *nfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg);

};


#endif //NFS_FUSE_VFSCLIENT_H
