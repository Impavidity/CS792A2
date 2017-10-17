#include "FileSystemInterface.h"

FileSystemInterface::FileSystemInterface() {

}

__ino_t FileSystemInterface::getInode(const std::string& path) {
    std::string fullPath = getFullPath(path);
    struct stat sb;
    if (::stat(fullPath.c_str(), &sb) == 0 /*&& S_ISDIR(sb.st_mode)*/) {
        return sb.st_ino;
    } else {
        return 0;
    }
}


int FileSystemInterface::readdir(const std::string& path, std::vector<thrift_dir_entry> &entries) {
    std::string fullPath = getFullPath(path);
    DIR *dp;
    struct dirent *de;

    dp = ::opendir(fullPath.c_str());
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

int FileSystemInterface::getattr(const std::string& path, thrift_stat &tstbuf) {
    std::string fullPath = getFullPath(path);
    int ret;
    struct stat stbuf;
    ret = ::lstat(fullPath.c_str(), &stbuf);
    statToThrift(&stbuf, tstbuf);
    if (ret == -1)
        ret = -ENOENT;
    return ret;
}

int FileSystemInterface::mkdir(const std::string& path, mode_t mode) {
    std::string fullPath = getFullPath(path);
    int ret;
    std::cout << "File Interface : mkdir " << fullPath << std::endl;
    ret = ::mkdir(fullPath.c_str(), mode);
    if (ret == -1)
        ret = -ENOENT;
    return ret;
}

int FileSystemInterface::rmdir(const std::string& path) {
    std::string fullPath = getFullPath(path);
    int ret;
    ret = ::rmdir(fullPath.c_str());
    if (ret == -1)
        ret = -ENOENT;
    return ret;
}

FileSystemInterface::FileSystemInterface(const std::string& root) {
    this->root = root;
}

std::string FileSystemInterface::getFullPath(const std::string& path) {
    return this->root + path;
}

int32_t FileSystemInterface::unlink(const std::string& path) {
    std::string fullPath = getFullPath(path);
    int res;
    res = ::unlink(fullPath.c_str());
    if (res == -1)
        return -errno;
    return 0;
}

int32_t FileSystemInterface::rename(const std::string& path, const std::string &toname) {
    std::string fullPath = getFullPath(path);
    // TODO implement
    return 0;
}

int32_t FileSystemInterface::create(const std::string& path) {
    std::string fullPath = getFullPath(path);
    std::ofstream os(path, std::ios::binary);
    os.close();
    return 0;
}

void FileSystemInterface::read(thrift_read_reply &_return, const std::string& path, const int64_t size, const int64_t offset) {
    std::string fullPath = getFullPath(path);
    std::ifstream is(path, std::ios::binary);
    is.seekg(offset, std::ios::beg);
    std::string buf(size, '\0');
    is.read(&buf[0], size);
    _return.buf = buf;
    _return.ret = is.gcount();
    is.close();
}

int32_t FileSystemInterface::write(const std::string& path, const std::string &buf, const int64_t size, const int64_t offset) {
    std::string fullPath = getFullPath(path);
    std::ofstream os(path, std::ios::binary);
    os.seekp(offset, std::ios::beg);
    os.write(&buf[0], size);
    int32_t ret = os.tellp() - offset;
    os.close();
    return ret;
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
