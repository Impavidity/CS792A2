//
// Created by shipeng on 17-10-9.
//

#include "FileSystemInterface.h"



using namespace std;

FileSystemInterface::FileSystemInterface() {

}

__ino_t FileSystemInterface::getInode(const char *path) {
  struct stat sb;
  if (::stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
    return sb.st_ino;
  } else {
    return 0;
  }
}


int FileSystemInterface::readdir(const char *path, std::vector<thrift_dir_entry> &entries) {
  DIR *dp;
  struct dirent *de;

  dp = ::opendir(path);
  if (dp == NULL)
    return -ENOENT;

  while ((de=::readdir(dp)) != NULL) {
    thrift_dir_entry entry;
    entry.__set_dir_id(de->d_ino);
    entry.__set_dir_name(de->d_name);
    entry.__set_dir_type(de->d_type);
    entries.push_back(entry);
  }
  ::closedir(dp);
  return 0;
}

int FileSystemInterface::getattr(const char *path, struct stat *stbuf) {
  int ret;
  ret = ::lstat(path, stbuf);
  if (ret == -1)
    ret = -ENOENT;
  return ret;
}

int FileSystemInterface::mkdir(const char *path, mode_t mode) {
  int ret;
  ret = ::mkdir(path, mode);
  if (ret == -1)
    ret = -ENOENT;
  return ret;
}

int FileSystemInterface::rmdir(const char *path) {
  int ret;
  ret = ::rmdir(path);
  if (ret == -1)
    ret = -ENOENT;
  return ret;
}