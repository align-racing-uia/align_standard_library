cmake_minimum_required(VERSION 3.22)

project(asl)

add_library(asl INTERFACE)

add_subdirectory(asl INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/rtt
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/align
)

target_link_libraries(asl INTERFACE
    rtt
    align
)
