cmake_minimum_required(VERSION 3.30)
project(search_engine)

set(CMAKE_CXX_STANDARD 23)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

#add_executable(search_engine src/main.cpp)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)


target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(test)



