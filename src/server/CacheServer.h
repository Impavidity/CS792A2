//
// Created by zalsader on 14/10/17.
//

#ifndef NFS_FUSE_CACHESERVER_H
#define NFS_FUSE_CACHESERVER_H

#include "VNodeServer.h"
#include <unordered_map>
#include "NFS.h"

class CacheServer {
private:
    std::unordered_map<int64_t, VNodeServer> vnodes;
    int32_t systemId;
public:
    CacheServer(){};
    std::string getPath(const thrift_file_handler& fh);
    int32_t getSystemId();
    void get(thrift_file_handler &fh, int64_t inode);
    void add(thrift_file_handler &fh, int64_t inode, const std::string& fullPath);
    void remove(const thrift_file_handler& fh);
    void rename(const thrift_file_handler& fh, const std::string& name);
};


#endif //NFS_FUSE_CACHESERVER_H
