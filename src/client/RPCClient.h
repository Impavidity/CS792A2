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
  int nfs_create(const char* path, mode_t mode, struct fuse_file_info* fi);
  int nfs_readdir(const char* path, void * buf, fuse_fill_dir_t filler, off_t offset,
                  struct fuse_file_info *fi, enum fuse_readdir_flags flags);
  int nfs_mkdir(const char* path, mode_t mode);
  int nfs_rmdir(const char* path);
  int nfs_getattr(const char* path, struct stat* stbuf, struct fuse_file_info *fi);

};




#endif //NFS_FUSE_NFSCLIENT_H
