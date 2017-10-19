#include "RPCServer.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "Please specify <port> <mount_point>" << std::endl;
        exit(0);
    }
    std::srand(std::time(0));
    int port = atoi(argv[1]);
    std::string mountPoint = argv[2];
    bool allowCommit = true;
    if (argc >= 4) {
        allowCommit = (atoi(argv[3]) != 0);
    }
    RPCServer server(port, mountPoint, allowCommit);
    server.start();
    return 0;
}