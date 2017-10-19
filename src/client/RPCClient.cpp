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
  TSocket* socket = new TSocket(this->hostName, this->port);
  //socket->setRecvTimeout(500);
  //socket->setSendTimeout(500);
  this->socket = boost::shared_ptr<TTransport>(socket);
  this->transport = boost::shared_ptr<TTransport>(new TBufferedTransport(this->socket));
  this->protocol = boost::shared_ptr<TProtocol>(new TBinaryProtocol(this->transport));
  this->client = NFSClient(this->protocol);
  while (true) {
    try {
      this->transport->open();
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      continue;
    }
  }
}

thrift_file_handler_reply RPCClient::mount(const char *path) {
  thrift_file_handler_reply fh;
  std::string tpath(path);
  while (true) {
    try {
      this->client.mount(fh, tpath);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }

  return fh;
}

thrift_file_handler_reply RPCClient::lookup(thrift_file_handler fh, std::string tpath) {
  thrift_file_handler_reply new_fh;
  while (true) {
    try {
      this->client.lookup(new_fh, fh, tpath);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return new_fh;
}

thrift_getattr_reply RPCClient::nfs_getattr(thrift_file_handler fh) {
  thrift_getattr_reply reply;
  while (true) {
    try {
      this->client.getattr(reply, fh);
      break;
    } catch (TException &tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return reply;
}

thrift_readdir_reply RPCClient::nfs_readdir(thrift_file_handler fh) {
  thrift_readdir_reply reply;
  while (true) {
    try {
      this->client.readdir(reply, fh);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return reply;
}

thrift_file_handler_reply RPCClient::nfs_mkdir(thrift_file_handler fh, std::string name) {
  thrift_file_handler_reply reply;
  while (true) {
    try {
      this->client.mkdir(reply, fh, name);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return reply;
}

int RPCClient::nfs_rmdir(thrift_file_handler fh) {
  int ret;
  while (true) {
    try {
      ret = this->client.rmdir(fh);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return ret;
}

thrift_read_reply RPCClient::nfs_read(thrift_file_handler fh, int64_t size, int64_t offset) {
  thrift_read_reply reply;
  while (true) {
    try {
      this->client.read(reply, fh, size, offset);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return reply;
}

thrift_write_reply RPCClient::nfs_write(thrift_file_handler fh, const char *buf, int64_t size, int64_t offset) {
  thrift_write_reply reply;
  while (true) {
    try {
      this->client.write(reply, fh, std::string(buf), size, offset);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return reply;
}

thrift_file_handler_reply RPCClient::nfs_create(thrift_file_handler fh, std::string name) {
  thrift_file_handler_reply reply;
  while (true) {
    try {
      this->client.create(reply,fh, name);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return reply;
}

int RPCClient::nfs_unlink(thrift_file_handler fh) {
  int ret;
  while (true) {
    try{
      ret = this->client.unlink(fh);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return ret;
}

thrift_write_reply RPCClient::nfs_fsync(thrift_file_handler fh) {
  thrift_write_reply reply;
  while (true) {
    try{
      this->client.fsync(reply, fh);
      break;
    } catch (TException& tx) {
      std::cout << "ERROR: " << tx.what() << std::endl;
      pooling();
      continue;
    }
  }
  return reply;
}

void RPCClient::pooling() {
  transport->close();
  while (true)
    try{
      transport->open();
      break;
    } catch (TException& tx) {
      transport->close();
      usleep(1000000);
      continue;
    }
}

//int RPCClient::nfs_create(const char *path, mode_t mode, struct fuse_file_info* fi) {
//  //this->client.ping();
//  std::cout<<"Hello"<<std::endl;
//}



