#ifndef NFS_FUSE_FUSEINTERFACE_H
#define NFS_FUSE_FUSEINTERFACE_H

#include <vector>
#include <iostream>
#include <fstream>
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

    explicit FileSystemInterface(const std::string& root);

    std::string getFullPath(const std::string& path);

    __ino_t getInode(const std::string& path);

    int readdir(const std::string& path, std::vector<thrift_dir_entry> &entries);

    int getattr(const std::string& path, thrift_stat &tstbuf);

    int mkdir(const std::string& path, mode_t mode);

    int rmdir(const std::string& path);

    int32_t unlink(const std::string& path);

    int32_t rename(const std::string& path, const std::string &toname);

    int32_t create(const std::string& path);

    void read(thrift_read_reply &_return, const std::string& path, const int64_t size, const int64_t offset);

    int32_t write(const std::string& path, const std::string &buf, const int64_t size, const int64_t offset);
};


#endif //NFS_FUSE_FUSEINTERFACE_H
