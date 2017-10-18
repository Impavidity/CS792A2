//
// Created by zalsader on 14/10/17.
//

#include "CacheServer.h"

CacheServer::CacheServer(const std::string& root) {
    cacheFileName = root + '/' + CACHE_FILE_NAME;
    std::ifstream ifs(cacheFileName);
    if (ifs.good()) {
        boost::archive::text_iarchive ia(ifs);
        ia >> *this;
    } else {
        systemId = std::rand();
        persist();
    }
}

std::string CacheServer::getPath(const thrift_file_handler &fh) {
    for (auto pair : vnodes) {
        std::cout << pair.first << " --- " << pair.second.getPath() << std::endl;
    }
    if (fh.system_id != systemId) {
        throw -1;
    }
    if (vnodes.find(fh.inode) == vnodes.end()) {
        throw -1;
    }
    VNodeServer vn = vnodes[fh.inode];
    if (vn.getGeneration() != fh.generation_number) {
        throw -1;
    }
    return vn.getPath();
}

/**
 * Gets file handler or adds it to cache
 * @param fh
 * @param inode
 * @param fullPath
 */
void CacheServer::get(thrift_file_handler &fh, int64_t inode) {
    auto vn = vnodes.find(inode);
    if (vn == vnodes.end()) {
        throw -1;
    }
    fh.inode = inode;
    fh.generation_number = vn->second.getGeneration();
    fh.system_id = getSystemId();
}

void CacheServer::add(thrift_file_handler &fh, int64_t inode, const std::string& fullPath) {
    VNodeServer vn(fullPath);
    vnodes[inode] = vn;
    fh.inode = inode;
    fh.generation_number = vn.getGeneration();
    fh.system_id = getSystemId();
    persist();
}

int32_t CacheServer::getSystemId() {
    return systemId;
}

void CacheServer::remove(const thrift_file_handler &fh) {
    vnodes.erase(fh.inode);
    persist();
}

void CacheServer::rename(const thrift_file_handler &fh, const std::string &name) {
    auto vn = vnodes.find(fh.inode);
    if (vn == vnodes.end()) {
        throw -1;
    }
    // TODO implement
}

void CacheServer::persist() {
    std::ofstream ofs(cacheFileName);
    boost::archive::text_oarchive oa(ofs);
    oa << *this;
}
