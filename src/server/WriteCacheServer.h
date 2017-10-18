//
// Created by zalsader on 17/10/17.
//

#ifndef NFS_FUSE_WRITECACHESERVER_H
#define NFS_FUSE_WRITECACHESERVER_H

#include <unordered_map>
#include <vector>
#include "WriteCacheServerEntry.h"

class WriteCacheServer {
private:
    std::unordered_map<std::string, std::vector<WriteCacheServerEntry>> writeEntries;
public:
    void write(const std::string& path, const std::string &buf, const int64_t size, const int64_t offset);
    std::vector<WriteCacheServerEntry> getWriteEntries(std::string &path); // TODO return reference
    void clearWriteEntries(std::string &path);
};


#endif //NFS_FUSE_WRITECACHESERVER_H
