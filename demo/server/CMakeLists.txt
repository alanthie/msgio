cmake_minimum_required(VERSION 3.1)
project(msg_server)

include_directories(/home/allaptop/dev/libevent/include)
link_directories(/home/allaptop/dev/libevent/build/lib)

set (SRC msg_server.cpp)
set (HDR)

add_subdirectory(../../source build/msg_ui)

add_executable(msg_server
    ${SRC} ${HDR})

target_link_libraries(msg_server msg_io event event_pthreads)
