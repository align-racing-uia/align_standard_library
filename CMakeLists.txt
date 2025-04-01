cmake_minimum_required(VERSION 3.22)

project(asl)

add_library(asl INTERFACE)
add_subdirectory(libs/rtt)

target_include_directories(asl INTERFACE
    inc
)

target_sources(asl INTERFACE
    src/aligncan.c
    src/alignutils.c
)

target_link_libraries(asl INTERFACE
    rtt
)
