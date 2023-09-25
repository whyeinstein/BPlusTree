
set(CMAKE_C_COMPILER gcc )
set(CMAKE_CXX_COMPILER g++)

#set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -fno-inline") ？？？valgrind？？？ 从v

#gprof
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

cmake_minimum_required(VERSION 3.20.1)
project(b+tree)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)



add_subdirectory(src) 
add_subdirectory(test)