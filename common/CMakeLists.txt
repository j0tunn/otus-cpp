set(CMAKE_CXX_STANDARD 17)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

function(setupGTest target)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
  )

  FetchContent_MakeAvailable(googletest)

  include_directories(${googletest_SOURCE_DIR}/googlemock/include)
  target_link_libraries(${target} GTest::gtest_main GTest::gmock_main)

  include(GoogleTest)
  gtest_discover_tests(${target})
endfunction()


function(setupCPack target)
  install(TARGETS ${target} RUNTIME DESTINATION bin)

  set(CPACK_GENERATOR DEB)
  set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
  set(CPACK_PACKAGE_CONTACT j0tunn@ya.ru)
  include(CPack) # generates target `package`
endfunction()
