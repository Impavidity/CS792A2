//
// Created by shipeng on 17-10-8.
//


#include "RPCServer.h"


using boost::shared_ptr;

class NFSHandler : virtual public NFSIf {
private:
    CacheServer cacheServer;

    FileSystemInterface fileSystemInterface;

    thrift_file_handler rootFh;

public:
    NFSHandler(std::string root) : fileSystemInterface(root) {
        __ino_t inode = fileSystemInterface.getInode("/");
        cacheServer.getFileHandler(rootFh, inode, "/");
    }

    void root(thrift_file_handler& _return) {
        _return = rootFh;
        printf("root\n");
    }

    void mount(thrift_file_handler &_return, const std::string &path) {
        _return = rootFh;
        printf("mount\n");
    }

    void lookup(thrift_file_handler &_return, const thrift_file_handler &fh, const std::string &path) {
        std::string relPath = cacheServer.getPath(fh);
        relPath += '/' + path;
        __ino_t inode = fileSystemInterface.getInode(relPath.c_str());
        if (inode != 0) {
            cacheServer.getFileHandler(_return, inode, relPath);
        }
    }

    void readdir(thrift_readdir_reply &_return, const thrift_file_handler &fh) {
        std::string path = cacheServer.getPath(fh);
        std::vector<thrift_dir_entry> entries;
        _return.ret = fileSystemInterface.readdir(path, entries);
        _return.dir_entries = entries;
        printf("readdir\n");
    }

    int32_t mkdir(const thrift_file_handler &fh, const std::string &name) {
        // Your implementation goes here
        printf("mkdir\n");
    }

    int32_t rmdir(const thrift_file_handler &fh) {
        // Your implementation goes here
        printf("rmdir\n");
    }

    void getattr(thrift_getattr_reply &_return, const thrift_file_handler &fh) {
        std::string path = cacheServer.getPath(fh);
        int ret = fileSystemInterface.getattr(path, _return.tstbuf);
        _return.ret = ret;
        printf("getattr\n");
    }


};

RPCServer::RPCServer(int port, std::string mountRoot) {


    shared_ptr<NFSHandler> handler(new NFSHandler(mountRoot));
    this->processor = shared_ptr<TProcessor>(new NFSProcessor(handler));
    this->serverTransport = shared_ptr<TServerTransport>(new TServerSocket(port));
    this->transportFactory = shared_ptr<TTransportFactory>(new TBufferedTransportFactory());
    this->protocolFactory = shared_ptr<TProtocolFactory>(new TBinaryProtocolFactory());

    const int maxConnection = 64;

    this->threadFactory = shared_ptr<concurrency::PosixThreadFactory>(new concurrency::PosixThreadFactory());
    this->threadManager = concurrency::ThreadManager::newSimpleThreadManager(maxConnection);
    this->threadManager->threadFactory(this->threadFactory);
    this->threadManager->start();


}

void RPCServer::start() {
    TThreadPoolServer server(processor, serverTransport, transportFactory, protocolFactory, threadManager);
    server.serve();
}
