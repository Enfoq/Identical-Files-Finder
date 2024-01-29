cmake_minimum_required(VERSION 3.12)
project(IdenticalFilesFinder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Source)

# Collect source files
set(SOURCE_FILES
    Source/Private/FileParser.cpp
    Source/Private/FileWrapper.cpp
    Source/Main.cpp
)

# Add executable
add_executable(IdenticalFilesFinder ${SOURCE_FILES})