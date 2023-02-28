cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(mapreduce VERSION ${PROJECT_VERSION})

add_executable(mapreduce
    main.cpp
)

# Package
setupCPack(mapreduce)
