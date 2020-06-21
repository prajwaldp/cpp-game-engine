cmake_minimum_required(VERSION 3.5)

project(Ambient VERSION 1.0
                DESCRIPTION "A C++ game engine written from scratch"
                LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

cmake_policy(SET CMP0068 NEW)  # `RPATH` settings on macOS do not affect `ambient`

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)  # Includes the contents of the conanbuildinfo.cmake file.
conan_basic_setup()  # This is a macro defined in the above file

file(GLOB_RECURSE SOURCES src *.cpp)

add_library(ambient SHARED ${SOURCES})
target_include_directories(ambient PUBLIC include)
target_link_libraries(ambient ${CONAN_LIBS})

target_precompile_headers(ambient PUBLIC
                                  <algorithm>
                                  <array>
                                  <functional>
                                  <iostream>
                                  <memory>
                                  <sstream>
                                  <string>
                                  <vector>)
