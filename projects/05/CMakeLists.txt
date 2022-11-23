cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(editor VERSION ${PROJECT_VERSION})

add_executable(editor
    main.cpp
)

# Package
setupCPack(editor)
