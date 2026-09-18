cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0091 NEW)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(mygame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PRODUCTION_BUILD "Make this a production build" OFF)

# Add subdirectories
add_subdirectory(thirdparty/glfw-3.3.2 SYSTEM)
add_subdirectory(thirdparty/glad SYSTEM)
add_subdirectory(thirdparty/stb_image SYSTEM)
add_subdirectory(thirdparty/stb_truetype SYSTEM)
add_subdirectory(thirdparty/raudio SYSTEM)
add_subdirectory(thirdparty/glm SYSTEM)
add_subdirectory(thirdparty/imgui-docking SYSTEM)

# Collect sources
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE LIBRARY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/2D engine/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}" ${MY_SOURCES} ${LIBRARY_SOURCES})

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
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /arch:AVX2)
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" SYSTEM PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/2D engine/include"
)


target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE 
    glm 
    glfw 
    glad 
    stb_image 
    stb_truetype 
    raudio 
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