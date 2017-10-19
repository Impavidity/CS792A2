//
// Created by shipeng on 17-10-13.
//

#ifndef NFS_FUSE_CACHECLIENT_H
#define NFS_FUSE_CACHECLIENT_H

#include <map>
#include <vector>
#include <string>
#include "VNodeClient.h"
#include "WriteRecord.h"

class CacheClient {
public:
  CacheClient() {}
  std::map<std::string, VNodeClient> path2vnode;
  std::map<int64_t,std::vector<WriteRecord>> writeBuffer;
  int insertPath(std::string path, VNodeClient pv);
  int removePath(std::string path);
  VNodeClient* checkPath(std::string);
  int insertWriteRecord(int64_t inode, WriteRecord record);
  std::vector<WriteRecord>* getWriteVectorPT(int64_t inode);
  int flushWriteRecord(int64_t);
};


#endif //NFS_FUSE_CACHECLIENT_H
