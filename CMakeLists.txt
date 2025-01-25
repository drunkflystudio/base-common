cmake_minimum_required(VERSION 3.10)
project(Studio)

include(cmake/common.cmake)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT "BaseTests")

add_subdirectory(src)
