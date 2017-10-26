cmake_minimum_required(VERSION 3.6)
project(CList)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wno-unused-function")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-unused-function")

set(DEMO_SOURCE_FILES list.h demo.c)
add_executable(CListDemo ${DEMO_SOURCE_FILES})
set_property(TARGET CListDemo PROPERTY C_STANDARD 11)

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include)

set(TEST_SOURCE_FILES list.h CListTests.cpp)
add_executable(CListTests ${TEST_SOURCE_FILES})
set_property(TARGET CListTests PROPERTY CXX_STANDARD 11)
target_link_libraries(CListTests gtest_main)
