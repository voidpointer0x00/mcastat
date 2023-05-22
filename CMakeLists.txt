cmake_minimum_required(VERSION 3.22)
project(mcastat C)

set(CMAKE_C_STANDARD 99)

add_executable(mcastat main.c mcastat.c mcastat.h mca_region.h mca_region.c)
