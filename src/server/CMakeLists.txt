cmake_minimum_required(VERSION 3.5)
project(Server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(../rpc/gen-cpp)
#include_directories(../../libfuse/include)
#include_directories(../../libfuse/lib)

add_executable(server Server.cpp RPCServer.cpp FileSystemInterface.cpp ../rpc/gen-cpp/NFS_constants.cpp ../rpc/gen-cpp/NFS_types.cpp ../rpc/gen-cpp/NFS.cpp)
target_link_libraries(server -I gen-cpp -lboost_system -lpthread -lglog -lthrift)
