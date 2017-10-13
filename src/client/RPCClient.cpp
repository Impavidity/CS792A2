//
// Created by shipeng on 17-10-9.
//

#include "RPCClient.h"

void thrift2stat(thrift_stat &tstbuf, struct stat* stbuf) {
  stbuf->st_dev = (__dev_t) tstbuf.st_dev;
  stbuf->st_ino = (__ino_t) tstbuf.st_ino;
  stbuf->st_nlink = (__nlink_t) tstbuf.st_nlink;
  stbuf->st_mode = (__mode_t) tstbuf.st_mode;
  stbuf->st_uid = (__uid_t) tstbuf.st_uid;
  stbuf->st_gid = (__gid_t) tstbuf.st_gid;
  stbuf->__pad0 = tstbuf.__pad0;
  stbuf->st_rdev = (__dev_t) tstbuf.st_rdev;
  stbuf->st_size = tstbuf.st_size;
  stbuf->st_blksize = tstbuf.st_blksize;
  stbuf->st_blocks = tstbuf.st_blocks;
  stbuf->st_atim.tv_nsec = tstbuf.st_atim.tv_nsec;
  stbuf->st_atim.tv_sec = tstbuf.st_atim.tv_sec;
  stbuf->st_mtim.tv_nsec = tstbuf.st_mtim.tv_nsec;
  stbuf->st_mtim.tv_sec = tstbuf.st_mtim.tv_sec;
  stbuf->st_ctim.tv_nsec = tstbuf.st_ctim.tv_nsec;
  stbuf->st_ctim.tv_sec = tstbuf.st_ctim.tv_sec;
  stbuf->__glibc_reserved[0] = tstbuf.__glibc_reserved0;
  stbuf->__glibc_reserved[1] = tstbuf.__glibc_reserved1;
  stbuf->__glibc_reserved[2] = tstbuf.__glibc_reserved2;
}

RPCClient::RPCClient() {
  // Initialize here
}

void RPCClient::setPort(int port) {
  this->port = port;
}

void RPCClient::setHostName(char *hostName) {
  this->hostName = strdup(hostName);
}

void RPCClient::setRootPath(char *rootPath) {
  this->rootPath = strdup(rootPath);
}



void RPCClient::startClient() {
  this->socket = boost::shared_ptr<TTransport>(new TSocket(this->hostName, this->port));
  this->transport = boost::shared_ptr<TTransport>(new TBufferedTransport(this->socket));
  this->protocol = boost::shared_ptr<TProtocol>(new TBinaryProtocol(this->transport));
  this->client = NFSClient(this->protocol);
  try {
    this->transport->open();
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  thrift_file_handler fh;
  client.nfs_mount(fh, rootPath);
  if (fh.inode == 0) {
    std::cout << "Dir does not exist" << std::endl;
    exit(1);
  }
}

int RPCClient::nfs_create(const char *path, mode_t mode, struct fuse_file_info* fi) {
  //this->client.ping();
  std::cout<<"Hello"<<std::endl;
}

int RPCClient::nfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi,
                           enum fuse_readdir_flags flags) {
  std::string tpath(path);
  thrift_readdir_reply reply;
  try {
    this->client.nfs_readdir(reply, tpath);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }

  for (thrift_dir_entry tde : reply.dir_entries) {
    filler(buf, tde.dir_name.c_str(), NULL, 0, FUSE_FILL_DIR_PLUS);
  }

  return reply.ret;
}

int RPCClient::nfs_mkdir(const char *path, mode_t mode) {
  std::string tpath(path);
  int ret;
  try {
    ret = this->client.nfs_mkdir(tpath, mode);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  return ret;
}

int RPCClient::nfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
  std::string tpath(path);
  thrift_getattr_reply reply;
  try {
    this->client.nfs_getattr(reply, tpath);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  if (reply.ret > -1) {
    thrift2stat(reply.tstbuf, stbuf);
  }
  return reply.ret;
}

int RPCClient::nfs_rmdir(const char *path) {
  std::string tpath(path);
  int ret;
  try {
    ret = this->client.nfs_rmdir(tpath);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  return ret;
}