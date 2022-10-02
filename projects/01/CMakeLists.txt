cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(helloworld VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(helloworld_cli main.cpp)
add_library(helloworld_lib lib.cpp)

target_link_libraries(helloworld_cli PRIVATE helloworld_lib)

# Tests
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

enable_testing() # generates target `test`

add_executable(lib_test lib_test.cc)
target_link_libraries(lib_test helloworld_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lib_test)

# Package
install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack) # generates target `package`
