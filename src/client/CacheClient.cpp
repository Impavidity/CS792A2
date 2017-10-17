//
// Created by shipeng on 17-10-13.
//

#include "CacheClient.h"


VNodeClient* CacheClient::checkPath(std::string tpath) {
  auto pos = path2vnode.find(tpath);
  if ( pos != path2vnode.end())
    return &(pos->second);
  return nullptr;
}

int CacheClient::insertPath(std::string path, VNodeClient pv) {
//  if (path.find(std::string(".Trash"))!=std::string::npos) {
//    return 0;
//  }
//  path2vnode[path] = pv;
  return 0;
}

int CacheClient::removePath(std::string path) {
  auto it = path2vnode.find(path);
  if (it != path2vnode.end())
    path2vnode.erase(it);
  return 0;
}
