cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)
set(CMAKE_CXX_STANDARD 20)

project(bulk_server VERSION ${PROJECT_VERSION})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED system regex)

add_executable(${PROJECT_NAME}
    main.cpp
    cmd_processor.cpp
    bulk_reader.cpp
    bulk_router.cpp
    flush_observer.cpp
    stream_logger.cpp
    file_logger.cpp
    reader_state.cpp
    thread_logger.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system Boost::regex)

# Package
setupCPack(${PROJECT_NAME})
