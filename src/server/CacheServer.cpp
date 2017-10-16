//
// Created by zalsader on 14/10/17.
//

#include "CacheServer.h"

std::string CacheServer::getPath(const thrift_file_handler &fh) {
    if (fh.system_id != systemId) {
        return ""; // TODO through exception instead
    }
    if (vnodes.find(fh.inode) == vnodes.end()) {
        return ""; // TODO through exception instead
    }
    VNodeServer vn = vnodes[fh.inode];
    //if (vn.getGeneration() != fh.generation_number) {
    //    return ""; // TODO through exception instead
    //}
    return vn.getPath();
}

/**
 * Gets file handler or adds it to cache
 * @param fh
 * @param inode
 * @param fullPath
 */
void CacheServer::get(thrift_file_handler &fh, int64_t inode) {
    if (vnodes.find(inode) == vnodes.end()) {
        fh.inode = 0; // TODO through exception instead
        return;
    }
    VNodeServer vn = vnodes[inode];
    fh.inode = inode;
    fh.generation_number = vn.getGeneration();
    fh.system_id = getSystemId();
}

void CacheServer::add(thrift_file_handler &fh, int64_t inode, std::string fullPath) {
    VNodeServer vn(fullPath);
    vnodes[inode] = vn;
    fh.inode = inode;
    fh.generation_number = vn.getGeneration();
    fh.system_id = getSystemId();
}

int32_t CacheServer::getSystemId() {
    // TODO remove this
    if (systemId == 0) {
        systemId = rand(); // TODO use another random function
    }
    // TODO read it from cache
    return systemId;
}

void CacheServer::remove(const thrift_file_handler &fh) {
    vnodes.erase(fh.inode);
}
