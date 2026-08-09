# ==============================================================================
# GRAVITAS - N-Body Simulation Engine
# ==============================================================================
cmake_minimum_required(VERSION 3.25)

project(Gravitas
  VERSION 1.0.0
  DESCRIPTION "High-performance N-Body simulation engine"
  LANGUAGES CXX
)

# ==============================================================================
# Global C++ Standards
# ==============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ==============================================================================
# Dependencies: Raylib, ImGui, rlImGui
# ==============================================================================
include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "Disable Raylib examples" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "Disable Raylib games" FORCE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        6.0
)

FetchContent_Declare(
  rlimgui
  GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
  GIT_TAG        Raylib_6_0
)

FetchContent_MakeAvailable(raylib rlimgui)

FetchContent_Populate(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.92.9b
)

file(GLOB THIRD_PARTY_SOURCES
  "${imgui_SOURCE_DIR}/*.cpp"
  "${rlimgui_SOURCE_DIR}/*.cpp"
)

# ==============================================================================
# Compiler Warnings & Diagnostics
# ==============================================================================
function(gravitas_apply_strict_warnings target_name)
  if(MSVC)
    target_compile_options(${target_name} PRIVATE
      /W4              # Baseline strict warnings
      /WX              # Treat warnings as errors
      /permissive-     # Enforce strict standard C++ conformance
      /Zc:preprocessor # Use the standard-compliant preprocessor
      /w14242          # Possible loss of data in conversion
      /w14263          # Member function does not override any base class virtual
      /w14265          # Class has virtual functions, but destructor is not virtual
      /w14287          # Unsigned/negative constant mismatch
      /w14296          # Expression is always 'boolean_value'
    )
  else()
    target_compile_options(${target_name} PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Werror         # Treat warnings as errors
      -Wconversion    # Catch implicit cast data loss
      -Wsign-conversion
      -Wshadow        # Catch shadowed variables
      -Wnon-virtual-dtor
    )
  endif()
endfunction()

# ==============================================================================
# Source Files Collection
# ==============================================================================
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
  "include/*.hpp"
  "src/*.cpp"
)

# ==============================================================================
# Target Setup
# ==============================================================================

# Compile Third-Party code separately
add_library(third_party STATIC ${THIRD_PARTY_SOURCES})

target_include_directories(third_party PUBLIC
  ${imgui_SOURCE_DIR}
  ${rlimgui_SOURCE_DIR}
)

target_link_libraries(third_party PUBLIC raylib)

# Compile Main Executable
add_executable(${PROJECT_NAME}
  Main.cpp
  ${ENGINE_SOURCES}
)

gravitas_apply_strict_warnings(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${imgui_SOURCE_DIR}
  ${rlimgui_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib third_party)
