#include "FileSystemInterface.h"

FileSystemInterface::FileSystemInterface() {

}

__ino_t FileSystemInterface::getInode(std::string path) {
    std::string fullPath = getFullPath(path);
    struct stat sb;
    if (::stat(fullPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return sb.st_ino;
    } else {
        return 0;
    }
}


int FileSystemInterface::readdir(std::string path, std::vector<thrift_dir_entry> &entries) {
    const char * fullPath = getFullPath(path).c_str();
    DIR *dp;
    struct dirent *de;

    dp = ::opendir(fullPath);
    if (dp == NULL)
        return -ENOENT;

    while ((de = ::readdir(dp)) != NULL) {
        thrift_dir_entry entry;
        entry.__set_dir_id(de->d_ino);
        entry.__set_dir_name(de->d_name);
        entry.__set_dir_type(de->d_type);
        entries.push_back(entry);
    }
    ::closedir(dp);
    return 0;
}

int FileSystemInterface::getattr(std::string path, thrift_stat &tstbuf) {
    const char * fullPath = getFullPath(path).c_str();

    int ret;
    struct stat stbuf;
    ret = ::lstat(fullPath, &stbuf);
    std::cout << " In system interface " << fullPath << stbuf.st_ino << std::endl;
    statToThrift(&stbuf, tstbuf);
    if (ret == -1)
        ret = -ENOENT;
    return ret;
}

int FileSystemInterface::mkdir(std::string path, mode_t mode) {
    const char * fullPath = getFullPath(path).c_str();
    int ret;
    ret = ::mkdir(fullPath, mode);
    if (ret == -1)
        ret = -ENOENT;
    return ret;
}

int FileSystemInterface::rmdir(std::string path) {
    const char * fullPath = getFullPath(path).c_str();
    int ret;
    ret = ::rmdir(fullPath);
    if (ret == -1)
        ret = -ENOENT;
    return ret;
}

FileSystemInterface::FileSystemInterface(std::string root) {
    this->root = root;
}

std::string FileSystemInterface::getFullPath(std::string path) {
    return this->root + path;
}

void FileSystemInterface::statToThrift(struct stat *stbuf, thrift_stat &tstbuf) {
    tstbuf.st_dev = (int64_t) stbuf->st_dev;
    tstbuf.st_ino = (int64_t) stbuf->st_ino;
    tstbuf.st_nlink = (int64_t) stbuf->st_nlink;
    tstbuf.st_mode = stbuf->st_mode;
    tstbuf.st_uid = stbuf->st_uid;
    tstbuf.st_gid = stbuf->st_gid;
    tstbuf.__pad0 = stbuf->__pad0;
    tstbuf.st_rdev = (int64_t) stbuf->st_rdev;
    tstbuf.st_size = (int64_t) stbuf->st_size;
    tstbuf.st_blksize = (int64_t) stbuf->st_blksize;
    tstbuf.st_blocks = (int64_t) stbuf->st_blocks;
    tstbuf.st_atim.tv_nsec = (int64_t) stbuf->st_atim.tv_nsec;
    tstbuf.st_atim.tv_sec = (int64_t) stbuf->st_atim.tv_sec;
    tstbuf.st_mtim.tv_nsec = (int64_t) stbuf->st_mtim.tv_nsec;
    tstbuf.st_mtim.tv_sec = (int64_t) stbuf->st_mtim.tv_sec;
    tstbuf.st_ctim.tv_nsec = (int64_t) stbuf->st_ctim.tv_nsec;
    tstbuf.st_ctim.tv_sec = (int64_t) stbuf->st_ctim.tv_sec;
    tstbuf.__glibc_reserved0 = (int64_t) stbuf->__glibc_reserved[0];
    tstbuf.__glibc_reserved1 = (int64_t) stbuf->__glibc_reserved[1];
    tstbuf.__glibc_reserved2 = (int64_t) stbuf->__glibc_reserved[2];
}
