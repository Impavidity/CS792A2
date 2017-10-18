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
    int64_t writeVerifier;
public:
    WriteCacheServer();
    void write(const std::string& path, const std::string &buf, const int64_t size, const int64_t offset);
    std::vector<WriteCacheServerEntry> getWriteEntries(std::string &path); // TODO return reference
    void clearWriteEntries(std::string &path);
    int64_t getWriteVerifier();
};


#endif //NFS_FUSE_WRITECACHESERVER_H
