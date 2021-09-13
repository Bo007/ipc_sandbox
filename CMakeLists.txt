cmake_minimum_required(VERSION 3.5)

project(multithreading LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Server.h)

add_executable(multithreading main.cpp ${HEADERS})

target_include_directories(multithreading
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/json/single_include/)
