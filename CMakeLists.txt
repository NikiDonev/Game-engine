cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0091 NEW)

set(CMAKE_CXX_STANDARD 17)

option(PRODUCTION_BUILD "Make this a production build" OFF)

# 1. Clear, single-line variable for MT / MTd. Must be BEFORE project()
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # link time optimization

if(MSVC) 
    add_compile_options(/arch:AVX2) # SIMD optimizations
endif()

project(mygame)

# ... GLFW/GLAD cache options stay here ...

add_subdirectory(thirdparty/glfw-3.3.2)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/stb_image)
add_subdirectory(thirdparty/stb_truetype)
add_subdirectory(thirdparty/raudio)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/imgui-docking)


# MY_SOURCES is defined to be a list of all the source files for my game 
# DON'T ADD THE SOURCES BY HAND, they are already added with this macro
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE LIBRARY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/2D engine/src/*.cpp")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1) 

if(PRODUCTION_BUILD)
    # setup the ASSETS_PATH macro to be in the root folder of your exe
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 
else()
    # This is useful to get an ASSETS_PATH in your IDE during development
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
endif()

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC SHADERS="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} )
target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${LIBRARY_SOURCES} )

if(MSVC) # If using the VS compiler...
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
   

    #add this line if you want to remove the console!
    #set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/2D engine/include")


target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE 
    glm 
    glfw 
    glad 
    stb_image 
    stb_truetype 
    raudio 
    imgui
)