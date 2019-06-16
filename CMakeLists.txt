# cmake_minimum_required(VERSION <specify CMake version here>)
project(ex4_os)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex4_os PhysicalMemory.h VirtualMemory.h MemoryConstants.h PhysicalMemory.cpp VirtualMemory.cpp  SimpleTest.cpp)

