cmake_minimum_required(VERSION 3.25)
project(stb_truetype)

add_library(stb_truetype STATIC)
target_sources(stb_truetype PRIVATE "src/stb_truetype.cpp")
target_include_directories(stb_truetype PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

