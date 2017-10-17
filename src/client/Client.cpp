//
// Created by shipeng on 17-10-8.
//

#include "RPCClient.h"
#include "VFSClient.h"


static VFSClient vfsClient;



struct nfs_fuse_operations : fuse_operations {
  nfs_fuse_operations () {
    init = vfsClient.nfs_init;
    getattr = vfsClient.getattr;
    readdir = vfsClient.nfs_readdir;
    mkdir = vfsClient.nfs_mkdir;
    rmdir = vfsClient.nfs_rmdir;
    read = vfsClient.nfs_read;
    open = vfsClient.nfs_open;
    //.create = nfs_create,
  }
};

static struct nfs_fuse_operations nfs_oper;


int main(int argc, char *argv[]) {
  if (argc < 5) {
    std::cout << "Please specify the <mount_point> <port> <hostname> <root_path>" << std::endl;
    exit(0);
  }
  std::cout << "Client Start ..." << std::endl;
  vfsClient.rpcClient.setPort(atoi(argv[argc-3]));
  vfsClient.rpcClient.setHostName(argv[argc-2]);
  vfsClient.rpcClient.setRootPath(argv[argc-1]);
  vfsClient.rpcClient.startClient();
  vfsClient.mount(argv[argc-1]);
  umask(0);
  return fuse_main(argc-3, argv, &nfs_oper, NULL);
}
