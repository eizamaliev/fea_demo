
file(GLOB srcs *.cpp)
file(GLOB hdrs *.hpp)

add_library(core STATIC ${srcs})