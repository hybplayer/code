cmake_minimum_required (VERSION 2.8)

project (demo)

include_directories(testfunc testfunc1)

aux_source_directory(testfunc SRC_LIST)
aux_source_directory(testfunc1 SRC_LIST1)

add_executable(main main.c ${SRC_LIST} ${SRC_LIST1})