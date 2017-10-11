cmake_minimum_required(VERSION 3.5)
project(RPC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


#add_executable(server NFS_server.skeleton.cpp gen-cpp/NFS_constants.cpp gen-cpp/NFS_types.cpp gen-cpp/NFS.cpp)
#add_executable(client NFS_client.skeleton.cpp gen-cpp/NFS_constants.cpp gen-cpp/NFS_types.cpp gen-cpp/NFS.cpp)
#target_link_libraries(server -I gen-cpp -lboost_system -lpthread -lglog -lthrift)
#target_link_libraries(client -I gen-cpp -lboost_system -lpthread -lglog -lthrift)
