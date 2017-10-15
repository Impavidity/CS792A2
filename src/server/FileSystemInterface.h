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



class FileSystemInterface {
public:
  FileSystemInterface();
  __ino_t check(const char *path);
  int fuse_readdir(const char *path, std::vector<thrift_dir_entry> &entries);
  int fuse_getattr(const char *path, struct stat* stbuf);
  int fuse_mkdir(const char *path, mode_t mode);
  int fuse_rmdir(const char *path);


};


#endif //NFS_FUSE_FUSEINTERFACE_H
