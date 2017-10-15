//
// Created by zalsader on 14/10/17.
//

#include "VNodeServer.h"

VNodeServer::VNodeServer(std::string path, int gen) {
    abs_path = path;
    generation = gen;
}

VNodeServer::VNodeServer(std::string path) {
    abs_path = path;
    generation = std::rand(); // TODO use a better random engine
}

std::string VNodeServer::getPath() {
    return abs_path;
}

int VNodeServer::getGeneration() {
    return generation;
}

template<class Archive>
void VNodeServer::serialize(Archive &ar, const unsigned int version) {
    ar & abs_path;
    ar & generation;
}
