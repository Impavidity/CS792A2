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

static bool checkTime(VNodeClient cache, VNodeClient update) {
  return false;
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
  thrift_file_handler_reply reply;
  bool first = false;
  for (std::string token: paths) {
    if (token == std::string("") && !first) {
      first = true;
      continue;
    }
    reply = rpcClient->lookup(cur_fh, token);
    cur_fh = reply.fh;
    if (reply.ret == -ENOENT) {
      std::cout << "Dir/File does not exist during lookup" << std::endl;
      cur_fh.inode = 0;
      break;
    }
  }
  VNodeClient re_vnode = VNodeClient(rpcClient);
  re_vnode.fh = cur_fh;
  return re_vnode;
}


VNodeClient VNodeClient::getattr(VNodeClient *vnode, std::string path, VNodeClient* cache) {
  VNodeClient lookup_vnode;
  if (cache != nullptr) {
    lookup_vnode = lookup(cache, std::string(""));
    if (lookup_vnode.fh.inode == 0) {
      lookup_vnode = lookup(vnode, path);
    }
  } else {
    lookup_vnode = lookup(vnode, path);
  }
  std::cout << "In the getattr, the lookup inode # is " << lookup_vnode.fh.inode << std::endl;
  if (lookup_vnode.fh.inode == 0) {
    lookup_vnode.getattr_reply.ret = -ENOENT;
    return lookup_vnode;
  }
  lookup_vnode.getattr_reply = rpcClient->nfs_getattr(lookup_vnode.fh);
  return lookup_vnode;
}

VNodeClient VNodeClient::readdir(VNodeClient *vnode, std::string path, VNodeClient* cache) {
  VNodeClient lookup_vnode;
  if (cache != nullptr) {
    lookup_vnode = getattr(cache, std::string(""));
    if (lookup_vnode.fh.inode == 0) {
      lookup_vnode = getattr(vnode, path);
    } else { // If the file handler is valid, then we check the cache is valid or not
      if (checkTime(*cache, lookup_vnode)) {
        return *cache;
      }
    }
  } else {
    lookup_vnode = getattr(vnode, path);
  }
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

VNodeClient VNodeClient::write(VNodeClient *vnode, std::string path, const char *buf, size_t size, off_t offset) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  lookup_vnode.write_reply = rpcClient->nfs_write(lookup_vnode.fh, buf, size, offset);
  return lookup_vnode;
}

VNodeClient VNodeClient::create(VNodeClient *vnode, std::string path, std::string name) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  lookup_vnode.create_reply = rpcClient->nfs_create(lookup_vnode.fh, name);
  return lookup_vnode;
}

VNodeClient VNodeClient::unlink(VNodeClient *vnode, std::string path) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  lookup_vnode.unlink_reply = rpcClient->nfs_unlink(lookup_vnode.fh);
  return lookup_vnode;
}

VNodeClient VNodeClient::fsync(VNodeClient *vnode, std::string path) {
  VNodeClient lookup_vnode = lookup(vnode, path);
  lookup_vnode.fsync_reply = rpcClient->nfs_fsync(lookup_vnode.fh);
  return lookup_vnode;
}
