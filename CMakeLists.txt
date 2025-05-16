cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    if (DEFINED ENV{"GITHUB_ENV"})
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
    elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "arm64")
        message("Running on Apple Silicon")
        set(CMAKE_OSX_ARCHITECTURES "arm64")
    else()
        message("Running on Intel Mac")
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    endif()
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(ModProfiles VERSION 1.0.0)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src/)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

CPMAddPackage("gh:kingminer7/uibuilder#main")
target_link_libraries(${PROJECT_NAME} UIBuilder)

setup_geode_mod(${PROJECT_NAME})
