//
// Created by shipeng on 17-10-8.
//

#ifndef NFS_FUSE_NODE_H
#define NFS_FUSE_NODE_H

#include <string>
class Node {
public:
  int nfiles = 0;
  std::string name;
  int mode;
  Node(std::string name, int mode);
  virtual std::ostream& dump(std::ostream& o) const {
    return o << "Node [name:" << name << ", mode:" << mode << "]; ";
  }
};


#endif //NFS_FUSE_NODE_H
