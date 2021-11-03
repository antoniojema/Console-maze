# CMake minimum version check
cmake_minimum_required(VERSION 3.18)

#C++ standard required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# (Windows) Use folders
IF (WIN32)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
ENDIF()

# Global variables
set(ProjectName "console_maze")
set(ExeName "console_maze")

# Project
project(${ProjectName})

# Add source code
add_subdirectory("src/")