cmake_minimum_required(VERSION 3.16)
project(hscopt_rng C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(hscopt_rng STATIC
  src/rng.c
)

target_include_directories(hscopt_rng PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#Warnings
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(hscopt_rng PRIVATE -Wall -Wextra -Wpedantic)
endif()
