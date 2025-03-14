cmake_minimum_required(VERSION 3.30)
project(testSearch_engine)

set(CMAKE_CXX_STANDARD 23)

add_executable(testSearch_engine test.cpp test1.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

target_link_libraries(testSearch_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(testSearch_engine)
