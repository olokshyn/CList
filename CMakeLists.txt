cmake_minimum_required(VERSION 3.6)
project(CList)

set(DEMO_SOURCE_FILES list.h demo.c)
add_executable(CListDemo ${DEMO_SOURCE_FILES})
set_property(TARGET CListDemo PROPERTY C_STANDARD 11)
