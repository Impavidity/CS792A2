//
// Created by shipeng on 17-10-8.
//

#ifndef NFS_FUSE_NFSSERVER_H
#define NFS_FUSE_NFSSERVER_H


#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <sys/stat.h>
#include "NFS.h"
#include "FileSystemInterface.h"
#include "CacheServer.h"
#include "WriteCacheServer.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

class RPCServer {
public:
  boost::shared_ptr<TProcessor> processor;
  boost::shared_ptr<TServerTransport> serverTransport;
  boost::shared_ptr<TTransportFactory> transportFactory;
  boost::shared_ptr<TProtocolFactory> protocolFactory;
  boost::shared_ptr<concurrency::PosixThreadFactory> threadFactory;
  boost::shared_ptr<concurrency::ThreadManager> threadManager;
  RPCServer(int port, std::string mountRoot, bool allowCommit);
  void start();
};






#endif //NFS_FUSE_NFSSERVER_H
