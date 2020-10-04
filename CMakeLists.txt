cmake_minimum_required(VERSION 3.2)

project(sem3)

set(CMAKE_C_FLAGS "-std=gnu11 -fPIC")
set(CMAKE_CXX_FLAGS "-std=gnu14")

find_package(PythonLibs 3.6 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

set(SOURCES
    file.c
)

python_add_module(matrix ${SOURCES})