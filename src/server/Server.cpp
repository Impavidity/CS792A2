//
// Created by shipeng on 17-10-8.
//

#include "RPCServer.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Please specify <port>" << std::endl;
    exit(0);
  }

  int port = atoi(argv[1]);
  RPCServer server(port);
  server.start();
  return 0;
}