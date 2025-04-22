cmake_minimum_required(VERSION 3.22)

project(asl)

add_library(asl INTERFACE)
add_library(asl_can INTERFACE)
add_library(asl_utils INTERFACE)
add_library(asl_events INTERFACE)

add_subdirectory(libs/rtt)

target_include_directories(asl INTERFACE
    inc
)
target_include_directories(asl_can INTERFACE
    inc/can
)
target_include_directories(asl_utils INTERFACE
    inc/utils
)

target_include_directories(asl_events INTERFACE
    inc/events
)

target_sources(asl_can INTERFACE
    src/aligncan.c
)
target_sources(asl_utils INTERFACE
    src/alignutils.c
)

target_sources(asl INTERFACE
    src/alignevents.c
)

target_link_libraries(asl INTERFACE
    rtt
)
