cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../common/CMakeLists.txt)
set(CMAKE_CXX_STANDARD 20)

project(join_server VERSION ${PROJECT_VERSION})

find_package(Boost REQUIRED system)

include(FetchContent)
FetchContent_Declare(sqlite3 URL "https://www.sqlite.org/snapshot/sqlite-snapshot-202205041843.tar.gz")
FetchContent_MakeAvailable(sqlite3)
if (NOT sqlite3_POPULATED)
    FetchContent_Populate(sqlite3)
endif()

add_executable(join_server
    main.cpp
    db.cpp
    "${sqlite3_SOURCE_DIR}/sqlite3.c"
)

target_include_directories(join_server PUBLIC "${sqlite3_SOURCE_DIR}")
target_link_libraries(join_server PRIVATE Boost::system)

# Package
setupCPack(join_server)
