cmake_minimum_required(VERSION 3.0)
project(server)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(src/)

set(LIB_SRC 
    src/logger.cpp
    src/utils.cpp
    src/config.cpp
    src/mutex.cpp
    src/utils.cpp
    src/thread.cpp
    )

set(LIB
    yaml-cpp 
    pthread)
    
add_library(server SHARED ${LIB_SRC})
add_executable(testThread test/testThread.cpp)
add_dependencies(testThread server)
target_link_libraries(testThread server ${LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)
