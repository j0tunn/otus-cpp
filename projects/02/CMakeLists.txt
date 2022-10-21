cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(ip_filter VERSION ${PROJECT_VERSION})

add_executable(ip_filter main.cpp)
add_library(utils_lib utils.cpp)

target_link_libraries(ip_filter PRIVATE utils_lib)

# Tests
enable_testing()
add_executable(utils_test utils_test.cc)
target_link_libraries(utils_test utils_lib)
setupGTest(utils_test)

# Package
setupCPack(ip_filter)
