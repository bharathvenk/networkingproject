cmake_minimum_required(VERSION 3.22)
project(Project_3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project3 FileChecker.cpp FileCopier.cpp main.cpp Project3NetworkingModifier.cpp Util.cpp)
