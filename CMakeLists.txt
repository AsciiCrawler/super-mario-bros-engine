cmake_minimum_required(VERSION 3.23.1)

project(main)

set(CMAKE_CXX_STANDAR 14)
set(CMAKE_CXX_STANDAR_REQUIRED True)
set(CMAKE_CXX_FLAGS "-std=c++14 -lSDL2 -ldl") #-O3 
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

find_package(SDL2 REQUIRED)
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

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(main ${SDL2_LIBRARIES})