# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/xy/BplusTree

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/xy/BplusTree/build

# Utility rule file for run_clang_tidy.

# Include any custom commands dependencies for this target.
include CMakeFiles/run_clang_tidy.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/run_clang_tidy.dir/progress.make

CMakeFiles/run_clang_tidy:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/root/xy/BplusTree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running clang-tidy on source files"
	clang-tidy -p /root/xy/BplusTree/build /root/xy/BplusTree/include/BNode.h /root/xy/BplusTree/include/BplusTree.h /root/xy/BplusTree/src/BplusTree.cpp /root/xy/BplusTree/src/main.cpp

run_clang_tidy: CMakeFiles/run_clang_tidy
run_clang_tidy: CMakeFiles/run_clang_tidy.dir/build.make
.PHONY : run_clang_tidy

# Rule to build all files generated by this target.
CMakeFiles/run_clang_tidy.dir/build: run_clang_tidy
.PHONY : CMakeFiles/run_clang_tidy.dir/build

CMakeFiles/run_clang_tidy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/run_clang_tidy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/run_clang_tidy.dir/clean

CMakeFiles/run_clang_tidy.dir/depend:
	cd /root/xy/BplusTree/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/xy/BplusTree /root/xy/BplusTree /root/xy/BplusTree/build /root/xy/BplusTree/build /root/xy/BplusTree/build/CMakeFiles/run_clang_tidy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/run_clang_tidy.dir/depend

