cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(matrix VERSION ${PROJECT_VERSION})

add_executable(matrix
    main.cpp
)

# Tests
add_executable(test
    matrix_test.cc
)
setupGTest(test)

# Package
setupCPack(matrix)
