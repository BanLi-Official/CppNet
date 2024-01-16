cmake_minimum_required(VERSION 3.22)   #指定camke最低版本
project(Server_ByThreadPool)   #指定项目名称

set(CMAKE_C_COMPILER gcc)   #设置编译器
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 11)  #设置c++版本

aux_source_directory(ThreadPool_cpp SRC_ThreadPool)  #获取项目中涉及的源文件
aux_source_directory(Server SRC_Server)

include_directories()   #导入库
include_directories(ThreadPool_cpp)
include_directories(Server)

find_package(Threads REQUIRED)   #寻找系统安装的pthread库



add_executable(TCPServer ${SRC_ThreadPool} ${SRC_Server})  #生成可执行文件

target_link_libraries(TCPServer Threads::Threads ws2_32) #链接到可执行文件中



