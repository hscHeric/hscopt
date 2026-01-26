cmake_minimum_required(VERSION 3.16)
project(hscopt_rng C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(hscopt_rng STATIC
  src/rng.c
  src/hho.c
)

target_include_directories(hscopt_rng PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Warnings
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(hscopt_rng PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Performance / vectorization / inlining
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(hscopt_rng PRIVATE
    -O3
    -ffast-math
    -fno-finite-math-only
    -fno-math-errno
    -fno-trapping-math
    -fstrict-aliasing
    -march=native
    -mtune=native
    -funroll-loops
    -fomit-frame-pointer
    -fno-semantic-interposition
    -fvisibility=hidden
  )

  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_ok OUTPUT ipo_err)
  if (ipo_ok)
    set_property(TARGET hscopt_rng PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
endif()

# Math library
target_link_libraries(hscopt_rng PUBLIC m)

# OpenMP (opcional)
find_package(OpenMP)
if (OpenMP_C_FOUND)
  target_link_libraries(hscopt_rng PUBLIC OpenMP::OpenMP_C)
  target_compile_definitions(hscopt_rng PUBLIC HSCOPT_HAVE_OPENMP=1)
else()
  target_compile_definitions(hscopt_rng PUBLIC HSCOPT_HAVE_OPENMP=0)
endif()

