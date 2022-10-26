cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(helloworld VERSION ${PROJECT_VERSION})

configure_file(version.h.in version.h)
add_executable(helloworld_cli main.cpp)
add_library(helloworld_lib lib.cpp)

target_link_libraries(helloworld_cli PRIVATE helloworld_lib)

# Tests
add_executable(test lib_test.cc)
target_link_libraries(test helloworld_lib GTest::gtest_main)
setupGTest(test)

# Package
setupCPack(helloworld_cli)
