//
// Created by zalsader on 17/10/17.
//

#ifndef NFS_FUSE_WRITECACHESERVERENTRY_H
#define NFS_FUSE_WRITECACHESERVERENTRY_H

#include <string>


class WriteCacheServerEntry {
public:
    const std::string buf;
    const int64_t size;
    const int64_t offset;
};


#endif //NFS_FUSE_WRITECACHESERVERENTRY_H
