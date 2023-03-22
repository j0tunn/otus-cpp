cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)

project(async VERSION ${PROJECT_VERSION})

add_library(async SHARED
    async.cpp
    bulk_reader.cpp
    stream_logger.cpp
    file_logger.cpp
    reader_state.cpp
    thread_logger.cpp
)

add_executable(async_cli
    main.cpp
)
target_link_libraries(async_cli async)

# Package
install(TARGETS async LIBRARY DESTINATION lib)
install(TARGETS async_cli RUNTIME DESTINATION bin)
setupDebPkg()
