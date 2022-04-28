cmake_minimum_required(VERSION 3.13)

project(Harf)
set(CMAKE_CXX_STANDARD 20)

include(cmake/prevent_in_source_builds.cmake)
#include(cmake/compiler_warnings.cmake)
include(cmake/external.cmake)

add_subdirectory(src)