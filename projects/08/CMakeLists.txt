cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(bayan VERSION ${PROJECT_VERSION})

find_package(Boost REQUIRED program_options)

add_library(bayan_lib
    cli.cpp
    scanner.cpp
    fs_adapter.cpp
    file_reader.cpp
    hash.cpp
)

target_link_libraries(bayan_lib PRIVATE Boost::program_options)

add_executable(bayan
    main.cpp
    fs.cpp
)

target_link_libraries(bayan PRIVATE bayan_lib)

# Tests
add_executable(test
    cli_test.cc
    scanner_test.cc
    fs_adapter_test.cc
    file_reader_decorator_test.cc
)
target_link_libraries(test bayan_lib Boost::program_options)

setupGTest(test)

# Package
setupCPack(bayan)
