//
// Created by shipeng on 17-10-8.
//

#ifndef NFS_FUSE_FILEHANDLER_H
#define NFS_FUSE_FILEHANDLER_H

#include "Node.h"

class FileHandler {
public:
  FileHandler(Node node);
private:
  Node node;
};


#endif //NFS_FUSE_FILEHANDLER_H
