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
  thrift_file_handler cur_fh = vnode->fh;
  for (std::string token: paths) {
    if (token == std::string("")) continue;
    cur_fh = rpcClient->lookup(cur_fh, token).fh;
    if (cur_fh.inode == 0) {
      std::cout << "Dir/File does not exist during lookup" << std::endl;
      break;
    }
  }
  VNodeClient re_vnode = VNodeClient(rpcClient);
  re_vnode.fh = cur_fh;
  return re_vnode;
}


VNodeClient VNodeClient::getattr(VNodeClient *vnode, std::string path) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  std::cout << "In the getattr, the lookup inode # is " << lookup_vnode.fh.inode << std::endl;
  if (lookup_vnode.fh.inode == 0) {
    lookup_vnode.getattr_reply.ret = -ENOENT;
    return lookup_vnode;
  }
  lookup_vnode.getattr_reply = rpcClient->nfs_getattr(lookup_vnode.fh);
  return lookup_vnode;
}

VNodeClient VNodeClient::readdir(VNodeClient *vnode, std::string path) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  std::cout << "In the readdir, the lookup inode # is " << lookup_vnode.fh.inode << std::endl;
  lookup_vnode.readdir_reply = rpcClient->nfs_readdir(lookup_vnode.fh);
  return lookup_vnode;
}

VNodeClient VNodeClient::mkdir(VNodeClient *vnode, std::string path, std::string name) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  lookup_vnode.mkdir_reply = rpcClient->nfs_mkdir(lookup_vnode.fh, name);
  return lookup_vnode;
}

VNodeClient VNodeClient::rmdir(VNodeClient *vnode, std::string path) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  lookup_vnode.rmdir_reply = rpcClient->nfs_rmdir(lookup_vnode.fh);
  return lookup_vnode;
}

VNodeClient VNodeClient::read(VNodeClient *vnode, std::string path, int64_t size, int64_t offset) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  lookup_vnode.read_reply = rpcClient->nfs_read(lookup_vnode.fh, size, offset);
  return lookup_vnode;
}

VNodeClient VNodeClient::open(VNodeClient *vnode, std::string path) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  if (lookup_vnode.fh.inode == 0) {
    lookup_vnode.open_state = -ENOENT;
  } else {
    // Check Access only?
    lookup_vnode.open_state = 0;
  }
  return lookup_vnode;
}

