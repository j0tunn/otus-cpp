cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(print_ip VERSION ${PROJECT_VERSION})

add_executable(print_ip
    main.cpp
)

# Tests
add_executable(test
    ip_to_string_test.cc
)
setupGTest(test)

# Package
setupCPack(print_ip)
