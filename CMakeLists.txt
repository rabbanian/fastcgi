cmake_minimum_required(VERSION 3.8)
project(fastcgi)

set(CMAKE_CXX_STANDARD 14)
find_package (Threads)

set(SOURCE_FILES examples/helloWorld.cpp src/request.cpp src/server.cpp src/responder.cpp src/connection.cpp)
add_executable(fastcgi ${SOURCE_FILES})

target_link_libraries (fastcgi ${CMAKE_THREAD_LIBS_INIT})