//
// Created by zalsader on 14/10/17.
//

#ifndef NFS_FUSE_CACHESERVER_H
#define NFS_FUSE_CACHESERVER_H

#include <fstream>
#include <unordered_map>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/string.hpp>
#include "NFS.h"
#include "VNodeServer.h"

class CacheServer {
private:
    friend class boost::serialization::access;
    const std::string CACHE_FILE_NAME = ".vnodecache";
    std::string cacheFileName;
    std::unordered_map<int64_t, VNodeServer> vnodes;
    int32_t systemId;
    void persist();
    template<class Archive> void serialize(Archive & ar, const unsigned int version){
        ar & systemId;
        ar & vnodes;
    }
public:
    explicit CacheServer(const std::string& root);
    std::string getPath(const thrift_file_handler& fh);
    int32_t getSystemId();
    void get(thrift_file_handler &fh, int64_t inode);
    void add(thrift_file_handler &fh, int64_t inode, const std::string& fullPath);
    void remove(const thrift_file_handler& fh);
    void rename(const thrift_file_handler& fh, const std::string& name);
};


#endif //NFS_FUSE_CACHESERVER_H
