cmake_minimum_required(VERSION 3.16)
project(hscopt C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ProjectSetup)

add_library(hscopt STATIC
  src/rng.c
  src/hho.c
  src/rvns.c
)

target_include_directories(hscopt PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

hscopt_setup_target(hscopt)

# Math library
target_link_libraries(hscopt PUBLIC m)
