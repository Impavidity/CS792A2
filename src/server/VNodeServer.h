//
// Created by zalsader on 14/10/17.
//

#ifndef NFS_FUSE_VNODESERVER_H
#define NFS_FUSE_VNODESERVER_H

#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class VNodeServer {
private:
    friend class boost::serialization::access;
    std::string abs_path;
    int generation;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
public:
    VNodeServer(){};
    VNodeServer(std::string path, int gen);
    explicit VNodeServer(std::string path);
    int getGeneration();
    std::string getPath();
};


#endif //NFS_FUSE_VNODESERVER_H
