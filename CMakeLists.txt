cmake_minimum_required(VERSION 3.5)
project(NFS_FUSE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



include_directories(libfuse/lib)
include_directories(libfuse/include)
include_directories(src/rpc/gen-cpp)

#add_subdirectory(src/rpc)
add_subdirectory(src/server)
add_subdirectory(src/client)
