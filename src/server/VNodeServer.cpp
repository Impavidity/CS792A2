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

VNodeServer::VNodeServer(const VNodeServer &vn) {
    generation = vn.generation;
    abs_path = vn.abs_path;
}

VNodeServer& VNodeServer::operator=(const VNodeServer &vn) {
    generation = vn.generation;
    abs_path = vn.abs_path;
    return *this;
}
