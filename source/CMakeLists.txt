cmake_minimum_required(VERSION 3.1)
project(msgio-lib)

# Detect operating systems
if (CMAKE_SYSTEM MATCHES "Linux*")
    set (TARGET_LINUX ON)
endif()

if (CMAKE_SYSTEM MATCHES "Darwin*")
    set (TARGET_OSX ON)
endif()

if (CMAKE_SYSTEM MATCHES "Windows*")
    set (TARGET_WIN ON)
endif()

include_directories(/home/allaptop/dev/libevent/include)
link_directories(/home/allaptop/dev/libevent/build/lib)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED 11)
set (SRC proto_client.cpp proto_server.cpp proto_utils.cpp)
set (HDR proto_client.h proto_server.h proto_utils.h)
add_library(msg_io STATIC ${SRC} ${HDR})

# Export includes
target_include_directories(msg_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (TARGET_LINUX)
    target_compile_definitions(msg_io PRIVATE -DTARGET_LINUX)
endif()

if (TARGET_OSX)
    target_compile_definitions(msg_io PRIVATE -DTARGET_OSX)
endif()

if (TARGET_WIN)
    target_compile_definitions(msg_io PRIVATE -DTARGET_WIN)
endif()
