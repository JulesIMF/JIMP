#+++
# 
# Copyright(c) 2021 JulesIMF, MIPT
#
# Module name :
#
#		CMakeLists.txt
#
# Abstract:
#
#		My default CMake.Allows to compile the project
#		changing only FILES(and other flags if needed).
#		
#		Build this project with "make" or "make mol".#		
#
#---

#
# Settings
#

cmake_minimum_required(VERSION 3.16)
project(JG LANGUAGES CXX)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(WARNING_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)

#
# Definitions
#

# add_definitions(NDEBUG)

set(OPT "-O2")
set(SANITIZE 0)

if(SANITIZE)
    set(ASAN "-fsanitize=address")
else()
    set(ASAN "")
endif()

set(CMAKE_CXX_FLAGS "${OPT} ${ASAN} -g")
set(SFML_BUILD_GRAPHICS true)

#
# Building
#

include_directories(includes)

add_library(JG STATIC
    Events.cpp
    Shape.cpp
	Window.cpp
	Widget.cpp
	Button.cpp
	Canvas.cpp
	Panel.cpp
)

target_link_libraries(JG sfml-graphics sfml-window sfml-system)