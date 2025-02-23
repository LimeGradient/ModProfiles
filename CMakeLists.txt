cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(ModProfiles VERSION 1.0.0)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

include_directories(src)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

CPMAddPackage("gh:camila314/uibuilder#cd0f953")
CPMAddPackage("gh:LimeGradient/ZipLib#8c676b1")
target_link_libraries(${PROJECT_NAME} UIBuilder ZipLib)

setup_geode_mod(${PROJECT_NAME})
