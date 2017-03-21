cmake_minimum_required(VERSION 3.6)
project(Quadcopter-Testing)

include_directories(Modules)
include_directories(Modules/Logger)
include_directories(Modules/LoggerLite)
include_directories(Modules/Autopilot)
include_directories(Modules/Grabber)
include_directories(libraries/PetitFS/src)
include_directories("C:/Users/Elijah/Documents/Arduino/libraries/SD-master")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Testing/test.cpp)
add_executable(Testing ${SOURCE_FILES})