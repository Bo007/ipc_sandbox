cmake_minimum_required(VERSION 3.5)

project(client_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ServerSettings.h)

add_executable(client Client.cpp ${HEADERS})
add_executable(server Server.cpp ${HEADERS})

target_include_directories(client
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/json/single_include/)

target_include_directories(server
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/json/single_include/)
