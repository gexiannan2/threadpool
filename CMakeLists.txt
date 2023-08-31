cmake_minimum_required(VERSION 3.8)

project(ZEROThreadPool)

# 设置C++标准为 C++ 11
set(CMAKE_CXX_STANDARD 11)

# 设置调试标志
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")

# 添加可执行文件
add_executable(${PROJECT_NAME} main.cpp zero_threadpool.cpp)

# 包含头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 链接pthread库
target_link_libraries(${PROJECT_NAME} pthread)

