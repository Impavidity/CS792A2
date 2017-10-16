//
// Created by shipeng on 17-10-13.
//

#include "VNodeClient.h"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

template<typename Out>
static void split(const std::string &s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

static std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

RPCClient* VNodeClient::rpcClient;

VNodeClient::VNodeClient(RPCClient *rpcClient) {
  this->rpcClient = rpcClient;
  this->getattr_state = 0;
  this->readdir_state = 0;

}

VNodeClient VNodeClient::lookup(VNodeClient *vnode, std::string path) {
  std::vector<std::string> paths = split(path, '/');
  std::cout << "Full Path for lookup " << path << " The size is " << paths.size() <<std::endl;
  for (auto token : paths) std::cout << token << std::endl;
  thrift_file_handler cur_fh = vnode->fh;
  for (std::string token: paths) {
    if (token == std::string("")) continue;
    cur_fh = rpcClient->lookup(cur_fh, token);
    if (cur_fh.inode == 0) {
      std::cout << "Dir/File does not exist during lookup" << std::endl;
    }
  }
  VNodeClient re_vnode = VNodeClient(rpcClient);
  re_vnode.fh = cur_fh;
  std::cout << cur_fh.inode << std::endl;
  std::cout << "Root file handler "<< vnode->fh.inode << std::endl;
  return re_vnode;
}


VNodeClient VNodeClient::getattr(VNodeClient *vnode, std::string path) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  std::cout << "In the getattr" << lookup_vnode.fh.inode << std::endl;
  lookup_vnode.getattr_reply = rpcClient->nfs_getattr(lookup_vnode.fh);
  return lookup_vnode;
}

VNodeClient VNodeClient::readdir(VNodeClient *vnode, std::string path) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  std::cout << "In the readdir" << lookup_vnode.fh.inode << std::endl;
  lookup_vnode.readdir_reply = rpcClient->nfs_readdir(lookup_vnode.fh);
  return lookup_vnode;
}

