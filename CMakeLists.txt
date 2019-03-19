cmake_minimum_required (VERSION 2.6)
project(sl_api)
include_directories(src/include)
file(GLOB SOURCES "src/*.cpp")
add_executable(sl_api ${SOURCES})

set(DATA_FOLDER "\"${PROJECT_SOURCE_DIR}/data/\"")
configure_file (
  "${PROJECT_SOURCE_DIR}/src/test/test_common.hpp.in"
  "${PROJECT_SOURCE_DIR}/src/test/test_common.hpp"
)

project(test_sl_api)
include_directories(src/include/unity)
include_directories(src/include/unity_fixture)
include_directories(src/test)
include_directories(src)
file(GLOB TEST_SOURCES "src/test/*.cpp" "src/xml*.cpp" "src/include/unity/*.c" "src/include/unity_fixture/*.c")
add_executable(test_sl_api ${TEST_SOURCES})
