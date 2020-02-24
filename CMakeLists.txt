cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

include(local_settings.cmake OPTIONAL)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

set(PROJECT_NAME_LIB tinylib)

project(${PROJECT_NAME_LIB} VERSION 0.0.0.1 LANGUAGES CXX)

file(GLOB HDR include/*.hpp;include/*.h)
file(GLOB SRC src/*.cpp)

add_library(${PROJECT_NAME_LIB} STATIC ${SRC} ${HDR})
set_property(TARGET ${PROJECT_NAME_LIB} PROPERTY CXX_STANDARD 14)
target_include_directories(${PROJECT_NAME_LIB} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

add_subdirectory("samples/basic")