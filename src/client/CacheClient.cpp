//
// Created by shipeng on 17-10-13.
//

#include "CacheClient.h"


VNodeClient* CacheClient::checkPath(std::string tpath) {
  auto pos = path2vnode.find(tpath);
  if ( pos != path2vnode.end())
    return &pos->second;
  return nullptr;
}

int CacheClient::insertPath(std::string path, VNodeClient pv) {
  path2vnode[path] = pv;
  return 0;
}
