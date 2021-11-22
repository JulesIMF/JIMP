#+++
# 
# Copyright (c) 2021 JulesIMF, MIPT
#
# Module name:
#
#		CMakeLists.txt
#
# Abstract:
#
#		My default CMake. Allows to compile the project
#		changing only FILES (and other flags if needed).
#		
#		Build this project with "make" or "make mol".#		
#
#---

#
# Settings
#

cmake_minimum_required(VERSION 3.16)
project(jimp LANGUAGES CXX)
# set(CMAKE_CXX_COMPILER )
set(CMAKE_CXX_STANDARD 17)
set(WARNING_FLAGS "-Wno-multichar -Wno-unused-result -Wno-narrowing")

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

set(CMAKE_CXX_FLAGS "${OPT} ${WARNING_FLAGS} ${ASAN} -g")
set(SFML_BUILD_GRAPHICS true)

#
# Building
#

find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)

add_subdirectory(JG)

include_directories(includes JG/includes)

add_executable(jimp
        main.cpp
		common/messages.cpp
		common/colors.cpp
		editor/Editor.cpp
		editor/Palette.cpp
		editor/Tool.cpp
		editor/ToolPicker.cpp
		editor/LayerSwitcher.cpp
		editor/Layer.cpp
		file/BMP.cpp
) 

find_library(JG libJG.a JG)

target_link_libraries(jimp JG sfml-graphics sfml-window sfml-system)