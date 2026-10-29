cmake_minimum_required(VERSION 3.25)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(mygame LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PRODUCTION_BUILD "Make this a production build" OFF)
option(ENABLE_AVX2 "Enable AVX2 code generation on x86" ON)

# Project rule: third-party libraries stay statically linked.
set(BUILD_SHARED_LIBS OFF)

# GLFW: library only (skips examples/tests/docs/install targets).
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

# Add subdirectories
add_subdirectory(thirdparty/glfw-3.3.2 SYSTEM)
add_subdirectory(thirdparty/glad SYSTEM)
add_subdirectory(thirdparty/stb_image SYSTEM)
add_subdirectory(thirdparty/stb_truetype SYSTEM)
add_subdirectory(thirdparty/glm SYSTEM)
add_subdirectory(thirdparty/imgui-docking SYSTEM)


if(APPLE)
  target_compile_options(glfw PRIVATE -Wno-deprecated-declarations)
endif()

# Collect sources
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE LIBRARY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/2D engine/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}" ${MY_SOURCES} ${LIBRARY_SOURCES} )

# Target Definitions
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1)

if(PRODUCTION_BUILD)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC SHADERS="./shaders/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1)
else()
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC SHADERS="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)
endif()


# Platform-specific options
if(MSVC)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

# MinGW: link the GCC runtime statically so the exe needs no extra DLLs next to it.
if(MINGW)
    target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE -static)
endif()

# macOS: OpenGL is deprecated but functional; silence the deprecation warnings.
if(APPLE)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE GL_SILENCE_DEPRECATION)
endif()

# AVX2 is x86-only: skip ARM (Apple Silicon, Windows on ARM) and Apple universal builds.
if(ENABLE_AVX2
   AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64|i[3-6]86)$"
   AND NOT (APPLE AND CMAKE_OSX_ARCHITECTURES MATCHES "arm64"))
    if(MSVC)
        target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /arch:AVX2)
    else()
        target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE -mavx2)
    endif()
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" SYSTEM PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/2D engine"
)


target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE 
    glm 
    glfw 
    glad 
    stb_image 
    stb_truetype 
    imgui
)

if(APPLE)
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE 
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework QuartzCore"
    )
endif()