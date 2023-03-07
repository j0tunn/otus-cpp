cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(fashion_mnist VERSION ${PROJECT_VERSION})

add_library(utils_lib
    utils.cpp
)

# catboost
include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_directories(utils_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/catboost/static)
target_link_libraries(utils_lib PUBLIC catboostmodel)

add_executable(${PROJECT_NAME}
    main.cpp
)
target_link_libraries(${PROJECT_NAME} utils_lib)

# Tests
add_executable(test
    model_test.cc
)
target_link_libraries(test PUBLIC utils_lib)

add_custom_target(test_fixtures
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/fixtures ${CMAKE_CURRENT_BINARY_DIR}/fixtures
)
add_dependencies(test test_fixtures)

setupGTest(test)

# Package
setupCPack(${PROJECT_NAME})
