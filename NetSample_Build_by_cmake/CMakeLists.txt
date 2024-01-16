cmake_minimum_required(VERSION 3.22)
project(Server_ByThreadPool)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 11)

aux_source_directory(ThreadPool_cpp SRC_ThreadPool)
aux_source_directory(Server SRC_Server)

include_directories()
include_directories(ThreadPool_cpp)
include_directories(Server)

find_package(Threads REQUIRED)



add_executable(TCPServer ${SRC_ThreadPool} ${SRC_Server})

target_link_libraries(TCPServer Threads::Threads ws2_32)



