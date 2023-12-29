cmake_minimum_required(VERSION 3.26.3)
project(Glad)

add_library(glad include/glad/gl.h include/KHR/khrplatform.h src/gl.c)
target_include_directories(glad PUBLIC include/)