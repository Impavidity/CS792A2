//
// Created by shipeng on 17-10-9.
//

#ifndef NFS_FUSE_NFSCLIENT_H
#define NFS_FUSE_NFSCLIENT_H

#define FUSE_USE_VERSION 31

#include <iostream>
#include <string>
#include <fuse.h>
#include <stdlib.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <sys/stat.h>
#include "NFS.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

class RPCClient {
public:
  char* hostName;
  int port;
  char* rootPath;
  boost::shared_ptr<TTransport> socket;
  boost::shared_ptr<TProtocol> protocol;
  boost::shared_ptr<TTransport> transport;
  NFSClient client;
  RPCClient();
  void setHostName(char* hostName);
  void setPort(int port);
  void setRootPath(char* rootPath);
  void startClient();
  thrift_file_handler_reply mount(const char* path);
  thrift_file_handler_reply lookup(thrift_file_handler fh, std::string path);
  thrift_getattr_reply nfs_getattr(thrift_file_handler fh);
  thrift_readdir_reply nfs_readdir(thrift_file_handler fh);
  thrift_file_handler_reply nfs_mkdir(thrift_file_handler fh, std::string name);
  int nfs_rmdir(thrift_file_handler fh);
  thrift_read_reply nfs_read(thrift_file_handler fh, int64_t size, int64_t offset);
  int nfs_write(thrift_file_handler fh, const char* buf, int64_t size, int64_t offset);
  thrift_file_handler_reply nfs_create(thrift_file_handler fh, std::string name);
  int nfs_unlink(thrift_file_handler fh);
  int nfs_fsync(thrift_file_handler fh);

};

static void thrift2stat(thrift_stat &tstbuf, struct stat* stbuf) {
  stbuf->st_dev = (__dev_t) tstbuf.st_dev;
  stbuf->st_ino = (__ino_t) tstbuf.st_ino;
  stbuf->st_nlink = (__nlink_t) tstbuf.st_nlink;
  stbuf->st_mode = (__mode_t) tstbuf.st_mode;
  stbuf->st_uid = (__uid_t) tstbuf.st_uid;
  stbuf->st_gid = (__gid_t) tstbuf.st_gid;
  stbuf->__pad0 = tstbuf.__pad0;
  stbuf->st_rdev = (__dev_t) tstbuf.st_rdev;
  stbuf->st_size = tstbuf.st_size;
  stbuf->st_blksize = tstbuf.st_blksize;
  stbuf->st_blocks = tstbuf.st_blocks;
  stbuf->st_atim.tv_nsec = tstbuf.st_atim.tv_nsec;
  stbuf->st_atim.tv_sec = tstbuf.st_atim.tv_sec;
  stbuf->st_mtim.tv_nsec = tstbuf.st_mtim.tv_nsec;
  stbuf->st_mtim.tv_sec = tstbuf.st_mtim.tv_sec;
  stbuf->st_ctim.tv_nsec = tstbuf.st_ctim.tv_nsec;
  stbuf->st_ctim.tv_sec = tstbuf.st_ctim.tv_sec;
  stbuf->__glibc_reserved[0] = tstbuf.__glibc_reserved0;
  stbuf->__glibc_reserved[1] = tstbuf.__glibc_reserved1;
  stbuf->__glibc_reserved[2] = tstbuf.__glibc_reserved2;
}



#endif //NFS_FUSE_NFSCLIENT_H
