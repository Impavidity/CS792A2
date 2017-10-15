//
// Created by shipeng on 17-10-9.
//

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
public:
  FileSystemInterface();
  __ino_t getInode(const char *path);
  int readdir(const char *path, std::vector<thrift_dir_entry> &entries);
  int getattr(const char *path, struct stat* stbuf);
  int mkdir(const char *path, mode_t mode);
  int rmdir(const char *path);


};


#endif //NFS_FUSE_FUSEINTERFACE_H
