#include "RPCServer.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "Please specify <port> <mount_point>" << std::endl;
        exit(0);
    }

    int port = atoi(argv[1]);
    std::string mountPoint = argv[2];
    RPCServer server(port, mountPoint);
    server.start();
    return 0;
}