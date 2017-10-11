//
// Created by shipeng on 17-10-8.
//

#include "RPCServer.h"

int main(int argc, char **argv) {

  int port = 9090;
  RPCServer server(port);
  server.start();
  return 0;
}