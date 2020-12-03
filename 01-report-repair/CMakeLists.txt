cmake_minimum_required(VERSION 3.13)

project(advent-of-code)

add_compile_options(
  "-std=c++20"
  "-Ofast"
  "-march=native"
  "-Wall"
  "-Werror"
)

add_executable(app
  ./source/main.cc
)
