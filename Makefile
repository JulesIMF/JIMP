# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/julesimf/source/Ded 3/JIMP"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/julesimf/source/Ded 3/JIMP"

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start "/home/julesimf/source/Ded 3/JIMP/CMakeFiles" "/home/julesimf/source/Ded 3/JIMP/CMakeFiles/progress.marks"
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start "/home/julesimf/source/Ded 3/JIMP/CMakeFiles" 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named jimp

# Build rule for target.
jimp: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 jimp
.PHONY : jimp

# fast build rule for target.
jimp/fast:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/build
.PHONY : jimp/fast

#=============================================================================
# Target rules for targets named JG

# Build rule for target.
JG: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 JG
.PHONY : JG

# fast build rule for target.
JG/fast:
	$(MAKE) -f JG/CMakeFiles/JG.dir/build.make JG/CMakeFiles/JG.dir/build
.PHONY : JG/fast

common/colors.o: common/colors.cpp.o

.PHONY : common/colors.o

# target to build an object file
common/colors.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/common/colors.cpp.o
.PHONY : common/colors.cpp.o

common/colors.i: common/colors.cpp.i

.PHONY : common/colors.i

# target to preprocess a source file
common/colors.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/common/colors.cpp.i
.PHONY : common/colors.cpp.i

common/colors.s: common/colors.cpp.s

.PHONY : common/colors.s

# target to generate assembly for a file
common/colors.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/common/colors.cpp.s
.PHONY : common/colors.cpp.s

common/messages.o: common/messages.cpp.o

.PHONY : common/messages.o

# target to build an object file
common/messages.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/common/messages.cpp.o
.PHONY : common/messages.cpp.o

common/messages.i: common/messages.cpp.i

.PHONY : common/messages.i

# target to preprocess a source file
common/messages.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/common/messages.cpp.i
.PHONY : common/messages.cpp.i

common/messages.s: common/messages.cpp.s

.PHONY : common/messages.s

# target to generate assembly for a file
common/messages.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/common/messages.cpp.s
.PHONY : common/messages.cpp.s

editor/Editor.o: editor/Editor.cpp.o

.PHONY : editor/Editor.o

# target to build an object file
editor/Editor.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Editor.cpp.o
.PHONY : editor/Editor.cpp.o

editor/Editor.i: editor/Editor.cpp.i

.PHONY : editor/Editor.i

# target to preprocess a source file
editor/Editor.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Editor.cpp.i
.PHONY : editor/Editor.cpp.i

editor/Editor.s: editor/Editor.cpp.s

.PHONY : editor/Editor.s

# target to generate assembly for a file
editor/Editor.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Editor.cpp.s
.PHONY : editor/Editor.cpp.s

editor/Layer.o: editor/Layer.cpp.o

.PHONY : editor/Layer.o

# target to build an object file
editor/Layer.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Layer.cpp.o
.PHONY : editor/Layer.cpp.o

editor/Layer.i: editor/Layer.cpp.i

.PHONY : editor/Layer.i

# target to preprocess a source file
editor/Layer.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Layer.cpp.i
.PHONY : editor/Layer.cpp.i

editor/Layer.s: editor/Layer.cpp.s

.PHONY : editor/Layer.s

# target to generate assembly for a file
editor/Layer.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Layer.cpp.s
.PHONY : editor/Layer.cpp.s

editor/LayerSwitcher.o: editor/LayerSwitcher.cpp.o

.PHONY : editor/LayerSwitcher.o

# target to build an object file
editor/LayerSwitcher.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/LayerSwitcher.cpp.o
.PHONY : editor/LayerSwitcher.cpp.o

editor/LayerSwitcher.i: editor/LayerSwitcher.cpp.i

.PHONY : editor/LayerSwitcher.i

# target to preprocess a source file
editor/LayerSwitcher.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/LayerSwitcher.cpp.i
.PHONY : editor/LayerSwitcher.cpp.i

editor/LayerSwitcher.s: editor/LayerSwitcher.cpp.s

.PHONY : editor/LayerSwitcher.s

# target to generate assembly for a file
editor/LayerSwitcher.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/LayerSwitcher.cpp.s
.PHONY : editor/LayerSwitcher.cpp.s

editor/Palette.o: editor/Palette.cpp.o

.PHONY : editor/Palette.o

# target to build an object file
editor/Palette.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Palette.cpp.o
.PHONY : editor/Palette.cpp.o

editor/Palette.i: editor/Palette.cpp.i

.PHONY : editor/Palette.i

# target to preprocess a source file
editor/Palette.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Palette.cpp.i
.PHONY : editor/Palette.cpp.i

editor/Palette.s: editor/Palette.cpp.s

.PHONY : editor/Palette.s

# target to generate assembly for a file
editor/Palette.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Palette.cpp.s
.PHONY : editor/Palette.cpp.s

editor/Tool.o: editor/Tool.cpp.o

.PHONY : editor/Tool.o

# target to build an object file
editor/Tool.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Tool.cpp.o
.PHONY : editor/Tool.cpp.o

editor/Tool.i: editor/Tool.cpp.i

.PHONY : editor/Tool.i

# target to preprocess a source file
editor/Tool.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Tool.cpp.i
.PHONY : editor/Tool.cpp.i

editor/Tool.s: editor/Tool.cpp.s

.PHONY : editor/Tool.s

# target to generate assembly for a file
editor/Tool.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/Tool.cpp.s
.PHONY : editor/Tool.cpp.s

editor/ToolPicker.o: editor/ToolPicker.cpp.o

.PHONY : editor/ToolPicker.o

# target to build an object file
editor/ToolPicker.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/ToolPicker.cpp.o
.PHONY : editor/ToolPicker.cpp.o

editor/ToolPicker.i: editor/ToolPicker.cpp.i

.PHONY : editor/ToolPicker.i

# target to preprocess a source file
editor/ToolPicker.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/ToolPicker.cpp.i
.PHONY : editor/ToolPicker.cpp.i

editor/ToolPicker.s: editor/ToolPicker.cpp.s

.PHONY : editor/ToolPicker.s

# target to generate assembly for a file
editor/ToolPicker.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/editor/ToolPicker.cpp.s
.PHONY : editor/ToolPicker.cpp.s

file/BMP.o: file/BMP.cpp.o

.PHONY : file/BMP.o

# target to build an object file
file/BMP.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/file/BMP.cpp.o
.PHONY : file/BMP.cpp.o

file/BMP.i: file/BMP.cpp.i

.PHONY : file/BMP.i

# target to preprocess a source file
file/BMP.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/file/BMP.cpp.i
.PHONY : file/BMP.cpp.i

file/BMP.s: file/BMP.cpp.s

.PHONY : file/BMP.s

# target to generate assembly for a file
file/BMP.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/file/BMP.cpp.s
.PHONY : file/BMP.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/main.cpp.s
.PHONY : main.cpp.s

plugins/Effect.o: plugins/Effect.cpp.o

.PHONY : plugins/Effect.o

# target to build an object file
plugins/Effect.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/Effect.cpp.o
.PHONY : plugins/Effect.cpp.o

plugins/Effect.i: plugins/Effect.cpp.i

.PHONY : plugins/Effect.i

# target to preprocess a source file
plugins/Effect.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/Effect.cpp.i
.PHONY : plugins/Effect.cpp.i

plugins/Effect.s: plugins/Effect.cpp.s

.PHONY : plugins/Effect.s

# target to generate assembly for a file
plugins/Effect.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/Effect.cpp.s
.PHONY : plugins/Effect.cpp.s

plugins/Plugin.o: plugins/Plugin.cpp.o

.PHONY : plugins/Plugin.o

# target to build an object file
plugins/Plugin.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/Plugin.cpp.o
.PHONY : plugins/Plugin.cpp.o

plugins/Plugin.i: plugins/Plugin.cpp.i

.PHONY : plugins/Plugin.i

# target to preprocess a source file
plugins/Plugin.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/Plugin.cpp.i
.PHONY : plugins/Plugin.cpp.i

plugins/Plugin.s: plugins/Plugin.cpp.s

.PHONY : plugins/Plugin.s

# target to generate assembly for a file
plugins/Plugin.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/Plugin.cpp.s
.PHONY : plugins/Plugin.cpp.s

plugins/Tool.o: plugins/Tool.cpp.o

.PHONY : plugins/Tool.o

# target to build an object file
plugins/Tool.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/Tool.cpp.o
.PHONY : plugins/Tool.cpp.o

plugins/Tool.i: plugins/Tool.cpp.i

.PHONY : plugins/Tool.i

# target to preprocess a source file
plugins/Tool.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/Tool.cpp.i
.PHONY : plugins/Tool.cpp.i

plugins/Tool.s: plugins/Tool.cpp.s

.PHONY : plugins/Tool.s

# target to generate assembly for a file
plugins/Tool.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/Tool.cpp.s
.PHONY : plugins/Tool.cpp.s

plugins/appinterface.o: plugins/appinterface.cpp.o

.PHONY : plugins/appinterface.o

# target to build an object file
plugins/appinterface.cpp.o:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/appinterface.cpp.o
.PHONY : plugins/appinterface.cpp.o

plugins/appinterface.i: plugins/appinterface.cpp.i

.PHONY : plugins/appinterface.i

# target to preprocess a source file
plugins/appinterface.cpp.i:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/appinterface.cpp.i
.PHONY : plugins/appinterface.cpp.i

plugins/appinterface.s: plugins/appinterface.cpp.s

.PHONY : plugins/appinterface.s

# target to generate assembly for a file
plugins/appinterface.cpp.s:
	$(MAKE) -f CMakeFiles/jimp.dir/build.make CMakeFiles/jimp.dir/plugins/appinterface.cpp.s
.PHONY : plugins/appinterface.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... jimp"
	@echo "... JG"
	@echo "... common/colors.o"
	@echo "... common/colors.i"
	@echo "... common/colors.s"
	@echo "... common/messages.o"
	@echo "... common/messages.i"
	@echo "... common/messages.s"
	@echo "... editor/Editor.o"
	@echo "... editor/Editor.i"
	@echo "... editor/Editor.s"
	@echo "... editor/Layer.o"
	@echo "... editor/Layer.i"
	@echo "... editor/Layer.s"
	@echo "... editor/LayerSwitcher.o"
	@echo "... editor/LayerSwitcher.i"
	@echo "... editor/LayerSwitcher.s"
	@echo "... editor/Palette.o"
	@echo "... editor/Palette.i"
	@echo "... editor/Palette.s"
	@echo "... editor/Tool.o"
	@echo "... editor/Tool.i"
	@echo "... editor/Tool.s"
	@echo "... editor/ToolPicker.o"
	@echo "... editor/ToolPicker.i"
	@echo "... editor/ToolPicker.s"
	@echo "... file/BMP.o"
	@echo "... file/BMP.i"
	@echo "... file/BMP.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... plugins/Effect.o"
	@echo "... plugins/Effect.i"
	@echo "... plugins/Effect.s"
	@echo "... plugins/Plugin.o"
	@echo "... plugins/Plugin.i"
	@echo "... plugins/Plugin.s"
	@echo "... plugins/Tool.o"
	@echo "... plugins/Tool.i"
	@echo "... plugins/Tool.s"
	@echo "... plugins/appinterface.o"
	@echo "... plugins/appinterface.i"
	@echo "... plugins/appinterface.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

