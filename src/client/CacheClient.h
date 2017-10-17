//
// Created by shipeng on 17-10-13.
//

#ifndef NFS_FUSE_CACHECLIENT_H
#define NFS_FUSE_CACHECLIENT_H

#include <map>
#include <vector>
#include <string>
#include "VNodeClient.h"
#include "../common/WriteRecord.h"

class CacheClient {
public:
  CacheClient() {}
  std::map<std::string, VNodeClient> path2vnode;
  //std::vector<WriteRecord*> writeBuffer;
  int insertPath(std::string path, VNodeClient pv);
  int removePath(std::string path);
  VNodeClient* checkPath(std::string);
//  int insertWriteRecord(WriteRecord* record);
//  int flushWriteRecord(WriteRecord* record);
};


#endif //NFS_FUSE_CACHECLIENT_H
