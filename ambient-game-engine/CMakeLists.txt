cmake_minimum_required(VERSION 3.5)

project(Ambient VERSION 1.0
                DESCRIPTION "A C++ game engine written from scratch"
                LANGUAGES CXX)

cmake_policy(SET CMP0068 NEW)  # `RPATH` settings on macOS do not affect `ambient`

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)  # Includes the contents of the conanbuildinfo.cmake file.
conan_basic_setup()  # This is a macro defined in the above file

file(GLOB SOURCES src/*.cpp)

add_library(ambient SHARED ${SOURCES})
target_include_directories(ambient PUBLIC include)
target_link_libraries(ambient ${CONAN_LIBS})
target_compile_features(ambient PUBLIC cxx_std_11)
target_precompile_headers(ambient
PUBLIC
<algorithm>
<functional>
<iostream>
<memory>
<sstream>
<string>
)
