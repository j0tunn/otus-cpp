cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(ip_filter main.cpp)
add_library(utils_lib utils.cpp)

target_link_libraries(ip_filter PRIVATE utils_lib)

# Tests
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

enable_testing() # generates target `test`

add_executable(utils_test utils_test.cc)
target_link_libraries(utils_test utils_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(utils_test)

# Package
install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack) # generates target `package`
