project(FEM C CXX)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_STATIC_LIBRARY_PREFIX "")

set(CMAKE_BINARY_DIR "${PROJECT_BINARY_DIR}/bin")

set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ...")

message("${CMAKE_BUILD_TYPE}")

foreach(artifact IN ITEMS ARCHIVE COMPILE_PDB LIBRARY PDB RUNTIME)
    set("CMAKE_${artifact}_OUTPUT_DIRECTORY" "${CMAKE_BINARY_DIR}/$<CONFIG>")
endforeach()

option(ENABLE_TESTS "Enables unittesting")

include_directories("${PROJECT_SOURCE_DIR}/../eigen-3.4.0")
include_directories("${PROJECT_SOURCE_DIR}/core")

add_subdirectory(core)
if(ENABLE_TESTS)
    add_subdirectory(unittests)
endif()

add_executable(fem_demo main.cpp)
target_link_libraries(fem_demo core)