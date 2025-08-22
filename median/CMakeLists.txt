cmake_minimum_required(VERSION 3.14)
project(median)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O2")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

add_library(median median.h median.cc)

add_executable(median_test median_test.cc)
target_link_libraries(median_test GTest::gmock_main median)
gtest_discover_tests(median_test)
