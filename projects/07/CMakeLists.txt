cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(cmd VERSION ${PROJECT_VERSION})

add_library(bulk_reader_lib
    bulk_reader.cpp
    stream_logger.cpp
    file_logger.cpp
    reader_state.cpp
)

add_executable(bulk
    main.cpp
    fs.cpp
)

target_link_libraries(bulk bulk_reader_lib)

# Tests
add_executable(test
    bulk_reader_test.cc
    stream_logger_test.cc
    file_logger_test.cc
    mock_fs.cpp
)
target_link_libraries(test bulk_reader_lib)

setupGTest(test)

# Package
setupCPack(bulk)
