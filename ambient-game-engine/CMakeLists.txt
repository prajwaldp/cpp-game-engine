cmake_minimum_required(VERSION 3.5)

project(Ambient VERSION 1.0
                DESCRIPTION "A C++ game engine written from scratch"
                LANGUAGES CXX)

file(GLOB SOURCES src/*.cpp)

add_library(ambient SHARED ${SOURCES})
target_include_directories(ambient PUBLIC include)
target_compile_features(ambient PUBLIC cxx_std_11)
