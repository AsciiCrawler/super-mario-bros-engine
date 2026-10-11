cmake_minimum_required(VERSION 3.23.1)

project(main)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Default to an optimized build when no build type is given
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT isMultiConfig AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(MARIO_NATIVE "Tune the build for the host CPU (-march=native)" OFF)

find_package(SDL3 REQUIRED CONFIG)
include_directories("./includes")
include_directories("./src")
include_directories("./")

file(GLOB sourceFiles
     "src/*.c"
     "src/*.cpp"
     "src/*/*.c"
     "src/*/*.cpp"
)

add_executable( main src/main.cpp
                glad.c
                ${sourceFiles})

include(CheckIPOSupported)
check_ipo_supported(RESULT ipoSupported OUTPUT ipoOutput)
if(ipoSupported)
    set_property(TARGET main PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(MARIO_NATIVE)
    target_compile_options(main PRIVATE -march=native)
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(main PRIVATE SDL3::SDL3 ${CMAKE_DL_LIBS})
