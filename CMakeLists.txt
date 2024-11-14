cmake_minimum_required(VERSION 3.16)
project(tcp_multiple_clients)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the shared header file
include_directories(${PROJECT_SOURCE_DIR})

# Create an executable for the server
add_executable(server
    main_server.cpp
    Server.h
    Server.cpp
    shared.h
)