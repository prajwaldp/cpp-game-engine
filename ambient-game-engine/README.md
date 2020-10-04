# Ambient Game Engine

A C++ Game Engine written from scratch

## Build instructions

Install conan packages

```shell
$ mkdir build
$ cd build
$ conan install ..
$ cd ..
```

This will generate the file `./build/conanbuildinfo.cmake` which is included in
`./CMakeLists.txt`.

```shell
$ cmake -S . -B build
$ cmake --build build
```

> The library is created in `./build/lib`. The `lib` extension is because of the
> `include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)` in `./CMakeLists.txt`
> which adds `set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)` and `set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)`
