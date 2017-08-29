cmake_minimum_required(VERSION 3.8)
project(fastcgi)
set (FASTCGI_VERSION_MAJOR 0)
set (FASTCGI_VERSION_MINOR 2)
set(CMAKE_CXX_STANDARD 14)

find_package (Threads)

set(CMAKE_CXX_FLAGS_RELEASE
        "-O3 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG
        "-Og -g -Wall")

set(SOURCE_FILES
        src/request.cpp
        src/server.cpp
        src/responder.cpp
        src/connection.cpp)
set(EXAMPLE_FILES
        examples/helloWorld.cpp
        examples/simplePost.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} examples/helloWorld.cpp)

target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})