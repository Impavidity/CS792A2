//
// Created by shipeng on 17-10-13.
//

#ifndef NFS_FUSE_VNODECLIENT_H
#define NFS_FUSE_VNODECLIENT_H


#include <bits/types.h>
#include <NFS_types.h>
#include "RPCClient.h"

class VNodeClient {
public:
  // state 0: un-init 1: init
  VNodeClient(RPCClient* rpcClient);
  VNodeClient() {}
  static RPCClient* rpcClient;
  int getattr_state;
  thrift_getattr_reply getattr_reply;
  int readdir_state;
  thrift_readdir_reply readdir_reply;
  thrift_file_handler_reply mkdir_reply;
  int rmdir_reply;
  thrift_file_handler fh;
  thrift_read_reply read_reply;
  int read_state;


  // Specify static function for VNode. Generally pass the VNode pointer as one argument
  static VNodeClient lookup(VNodeClient* vnode, std::string path);
  static VNodeClient getattr(VNodeClient* vnode, std::string path);
  static VNodeClient readdir(VNodeClient* vnode, std::string path);
  static VNodeClient mkdir(VNodeClient* vnode, std::string path, std::string name);
  static VNodeClient rmdir(VNodeClient* vnode, std::string path);
  static VNodeClient read(VNodeClient* vnode, std::string path, int64_t size, int64_t offset);
  static VNodeClient write(VNodeClient* vnode,std::string path);


};





#endif //NFS_FUSE_VNODECLIENT_H
