cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(allocator VERSION ${PROJECT_VERSION})

add_executable(allocator
    main.cpp
    memory_mgr.cpp
)

# Tests
add_executable(test
    allocator_test.cc
    memory_mgr_mock.cpp
    list_test.cc
)
setupGTest(test)

# Package
setupCPack(allocator)
