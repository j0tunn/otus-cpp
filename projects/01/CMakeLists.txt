cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(helloworld VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)
add_executable(helloworld_cli main.cpp)
add_library(helloworld_lib lib.cpp)

target_link_libraries(helloworld_cli PRIVATE helloworld_lib)

# Tests
enable_testing()
add_executable(lib_test lib_test.cc)
target_link_libraries(lib_test helloworld_lib GTest::gtest_main)
setupGTest(lib_test)

# Package
setupCPack(helloworld_cli)
