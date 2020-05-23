cmake_minimum_required(VERSION 3.5)

project(Ambient VERSION 1.0
                DESCRIPTION "A C++ game engine written from scratch"
                LANGUAGES CXX)

add_library(ambient SHARED src/test.cpp include/test.h)
target_include_directories(ambient PUBLIC include)
target_compile_features(ambient PUBLIC cxx_std_11)
