cmake_minimum_required(VERSION 3.22)

project(asl)

add_library(asl INTERFACE)

add_subdirectory(libs/rtt)
add_subdirectory(libs/align)


target_link_libraries(asl INTERFACE
    rtt
    align
)
