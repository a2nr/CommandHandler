cmake_minimum_required(VERSION 3.6)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX C ASM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory("${CMAKE_SOURCE_DIR}/../../googletest" ${CMAKE_BINARY_DIR}/googletest )

include_directories("${gtest_SOURCE_DIR}/include"
                    "${gmock_SOURCE_DIR}/include"
                    "${CMAKE_SOURCE_DIR}/../../CommandHandler")

add_executable(${ProjectId} initTest.cpp test.cpp )
target_link_libraries(${ProjectId} gtest gtest_main)