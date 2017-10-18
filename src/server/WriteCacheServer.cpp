//
// Created by zalsader on 17/10/17.
//

#include <iostream>
#include "WriteCacheServer.h"

WriteCacheServer::WriteCacheServer() {
    writeVerifier = std::rand();
}

std::vector<WriteCacheServerEntry> WriteCacheServer::getWriteEntries(std::string &path) {
    auto entries = writeEntries.find(path);
    if (entries == writeEntries.end()) {
        return std::vector<WriteCacheServerEntry>();
    }
    return entries->second;
}

void WriteCacheServer::clearWriteEntries(std::string &path) {
    writeEntries.erase(path);
}

void
WriteCacheServer::write(const std::string &path, const std::string &buf, const int64_t size, const int64_t offset) {
    auto entries = writeEntries.find(path);
    if (entries == writeEntries.end()) {
        writeEntries[path] = std::vector<WriteCacheServerEntry>();
    }
    writeEntries[path].push_back(
            WriteCacheServerEntry {buf, size, offset}
    );
}

int64_t WriteCacheServer::getWriteVerifier() {
    return writeVerifier;
}
