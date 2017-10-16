#ifndef NFS_FUSE_FUSEINTERFACE_H
#define NFS_FUSE_FUSEINTERFACE_H

#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include "NFS.h"
#include "CacheServer.h"

class FileSystemInterface {
private:
    std::string root;

    void statToThrift(struct stat *stbuf, thrift_stat &tstbuf);

public:
    FileSystemInterface();

    explicit FileSystemInterface(std::string root);

    std::string getFullPath(std::string path);

    __ino_t getInode(std::string path);

    int readdir(std::string path, std::vector<thrift_dir_entry> &entries);

    int getattr(std::string path, thrift_stat &tstbuf);

    int mkdir(std::string path, mode_t mode);

    int rmdir(std::string path);

};


#endif //NFS_FUSE_FUSEINTERFACE_H
