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
    int server_id;
public:
    CacheServer(){};
    std::string getPath(const thrift_file_handler& fh);
    void getFileHandler(thrift_file_handler& fh, int64_t inode, std::string fullPath);
};


#endif //NFS_FUSE_CACHESERVER_H
