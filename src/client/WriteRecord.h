//
// Created by shipeng on 17-10-13.
//

#ifndef NFS_FUSE_WRITERECORD_H
#define NFS_FUSE_WRITERECORD_H

#define FUSE_USE_VERSION 31

#include <string>
#include <fuse.h>

class WriteRecord {
public:
  WriteRecord(std::string path, std::string content, size_t size, off_t offset, int64_t write_verifier, fuse_file_info fi){
    this->path = path;
    this->content = content;
    this->write_verifier = write_verifier;
    this->size = size;
    this->offset = offset;
    this->fi = fi;
  }
  std::string path;
  std::string content;
  int64_t write_verifier;
  size_t size;
  off_t offset;
  fuse_file_info fi;

};


#endif //NFS_FUSE_WRITERECORD_H
