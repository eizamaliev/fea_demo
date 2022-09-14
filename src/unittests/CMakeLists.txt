cmake_minimum_required(VERSION 3.10)

set(GTEST_PROJECT Tests)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread") 

enable_testing()

find_package(GTest REQUIRED)

# The utils, test, and gtests directories

file(GLOB SOURCES **/*.cpp)
file(GLOB HEADERS **/*.hpp)

message("${SOURCES}")
include_directories("${PROJECT_SOURCE_DIR}/core")
link_directories("${PROJECT_BINARY_DIR}/bin")

add_executable(${GTEST_PROJECT} ${SOURCES})
target_link_libraries(${GTEST_PROJECT} PUBLIC
  gtest
  gtest_main
  core
)

add_test(${GTEST_PROJECT} ${GTEST_PROJECT})
set_tests_properties(${GTEST_PROJECT} PROPERTIES 
                     ENVIRONMENT
                     DATADIR=${PROJECT_SOURCE_DIR}/data)