cmake_minimum_required(VERSION 3.30)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(search_engine main.cpp
        ${MY_INCLUDE_DIR}/ConverterJSON.h ConverterJSON.cpp
        ${MY_INCLUDE_DIR}/InvertedIndex.h InvertedIndex.cpp
        ${MY_INCLUDE_DIR}/SearchServer.h SearchServer.cpp)

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})