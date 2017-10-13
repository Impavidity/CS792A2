cmake_minimum_required(VERSION 3.5)
project(Client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

include_directories(../rpc/gen-cpp)
include_directories(../../libfuse/include)
include_directories(../../libfuse/lib)

find_package(PkgConfig REQUIRED)
pkg_search_module(FUSE3 REQUIRED fuse3)

add_executable(client Client.cpp RPCClient.cpp VFSClient.cpp VNodeClient.cpp ../common/WriteRecord.cpp CacheClient.cpp ../rpc/gen-cpp/NFS_constants.cpp ../rpc/gen-cpp/NFS_types.cpp ../rpc/gen-cpp/NFS.cpp)
target_link_libraries(client ${FUSE3_LIBRARIES} -I gen-cpp -lboost_system -lpthread -lglog -lthrift -I ../../libfuse/build/lib/)
target_include_directories(client PUBLIC ${FUSE3_INCLUDE_DIRS})
target_compile_options(client PUBLIC ${FUSE3_CFLAGS_OTHER})
