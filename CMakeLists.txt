cmake_minimum_required(VERSION 3.5...3.31)
project(Common)

# If this line fails, you are building this project wrong.
# Clone <https://github.com/drunkflystudio/drunkflystudio> and follow instructions there.
include(../_cmake/common.cmake)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/pstdint")

add_subdirectory(lib)
add_subdirectory(tests)

register_test_runner(BaseTests t_base)

add_subdirectory(src)
