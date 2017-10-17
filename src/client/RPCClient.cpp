//
// Created by shipeng on 17-10-9.
//

#include "RPCClient.h"



RPCClient::RPCClient() : client(this->protocol){
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
}

thrift_file_handler_reply RPCClient::mount(const char *path) {
  thrift_file_handler_reply fh;
  std::string tpath(path);
  this->client.mount(fh, tpath);
  return fh;
}

thrift_file_handler_reply RPCClient::lookup(thrift_file_handler fh, std::string tpath) {
  thrift_file_handler_reply new_fh;
  try {
    this->client.lookup(new_fh, fh, tpath);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  return new_fh;
}

thrift_getattr_reply RPCClient::nfs_getattr(thrift_file_handler fh) {
  thrift_getattr_reply reply;
  try {
    this->client.getattr(reply, fh);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  return reply;
}

thrift_readdir_reply RPCClient::nfs_readdir(thrift_file_handler fh) {
  thrift_readdir_reply reply;
  try {
    this->client.readdir(reply, fh);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  return reply;
}

thrift_file_handler_reply RPCClient::nfs_mkdir(thrift_file_handler fh, std::string name) {
  thrift_file_handler_reply reply;
  try {
    this->client.mkdir(reply, fh, name);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  return reply;
}

int RPCClient::nfs_rmdir(thrift_file_handler fh) {
  int ret;
  try {
    ret = this->client.rmdir(fh);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  return ret;
}

thrift_read_reply RPCClient::nfs_read(thrift_file_handler fh, int64_t size, int64_t offset) {
  thrift_read_reply reply;
  try {
    this->client.read(reply, fh, size, offset);
  } catch (TException& tx) {
    std::cout << "ERROR: " << tx.what() << std::endl;
  }
  return reply;
}

//int RPCClient::nfs_mkdir(const char *path, mode_t mode) {
//  std::string tpath(path);
//  int ret;
//  thrift_file_handler fh;
//  try {
//    ret = this->client.mkdir(fh, tpath);
//  } catch (TException& tx) {
//    std::cout << "ERROR: " << tx.what() << std::endl;
//  }
//  return ret;
//}
//
//
//int RPCClient::nfs_create(const char *path, mode_t mode, struct fuse_file_info* fi) {
//  //this->client.ping();
//  std::cout<<"Hello"<<std::endl;
//}




//int RPCClient::nfs_rmdir(const char *path) {
//  std::string tpath(path);
//  int ret;
//  try {
//    ret = this->client.nfs_rmdir(tpath);
//  } catch (TException& tx) {
//    std::cout << "ERROR: " << tx.what() << std::endl;
//  }
//  return ret;
//}


