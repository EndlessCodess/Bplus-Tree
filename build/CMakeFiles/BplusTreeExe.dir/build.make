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

# Include any dependencies generated for this target.
include CMakeFiles/BplusTreeExe.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/BplusTreeExe.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/BplusTreeExe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BplusTreeExe.dir/flags.make

CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.o: CMakeFiles/BplusTreeExe.dir/flags.make
CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.o: ../src/BplusTree.cpp
CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.o: CMakeFiles/BplusTreeExe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/xy/BplusTree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.o"
	$(CMAKE_COMMAND) -E __run_co_compile --tidy="clang-tidy;-p;/root/xy/BplusTree/build;--extra-arg-before=--driver-mode=g++" --source=/root/xy/BplusTree/src/BplusTree.cpp -- /usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.o -MF CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.o.d -o CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.o -c /root/xy/BplusTree/src/BplusTree.cpp

CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/xy/BplusTree/src/BplusTree.cpp > CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.i

CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/xy/BplusTree/src/BplusTree.cpp -o CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.s

CMakeFiles/BplusTreeExe.dir/src/main.cpp.o: CMakeFiles/BplusTreeExe.dir/flags.make
CMakeFiles/BplusTreeExe.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/BplusTreeExe.dir/src/main.cpp.o: CMakeFiles/BplusTreeExe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/xy/BplusTree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/BplusTreeExe.dir/src/main.cpp.o"
	$(CMAKE_COMMAND) -E __run_co_compile --tidy="clang-tidy;-p;/root/xy/BplusTree/build;--extra-arg-before=--driver-mode=g++" --source=/root/xy/BplusTree/src/main.cpp -- /usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/BplusTreeExe.dir/src/main.cpp.o -MF CMakeFiles/BplusTreeExe.dir/src/main.cpp.o.d -o CMakeFiles/BplusTreeExe.dir/src/main.cpp.o -c /root/xy/BplusTree/src/main.cpp

CMakeFiles/BplusTreeExe.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BplusTreeExe.dir/src/main.cpp.i"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/xy/BplusTree/src/main.cpp > CMakeFiles/BplusTreeExe.dir/src/main.cpp.i

CMakeFiles/BplusTreeExe.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BplusTreeExe.dir/src/main.cpp.s"
	/usr/bin/clang++-14 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/xy/BplusTree/src/main.cpp -o CMakeFiles/BplusTreeExe.dir/src/main.cpp.s

# Object files for target BplusTreeExe
BplusTreeExe_OBJECTS = \
"CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.o" \
"CMakeFiles/BplusTreeExe.dir/src/main.cpp.o"

# External object files for target BplusTreeExe
BplusTreeExe_EXTERNAL_OBJECTS =

../bin/Debug/BplusTreeExe: CMakeFiles/BplusTreeExe.dir/src/BplusTree.cpp.o
../bin/Debug/BplusTreeExe: CMakeFiles/BplusTreeExe.dir/src/main.cpp.o
../bin/Debug/BplusTreeExe: CMakeFiles/BplusTreeExe.dir/build.make
../bin/Debug/BplusTreeExe: CMakeFiles/BplusTreeExe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/xy/BplusTree/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../bin/Debug/BplusTreeExe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BplusTreeExe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BplusTreeExe.dir/build: ../bin/Debug/BplusTreeExe
.PHONY : CMakeFiles/BplusTreeExe.dir/build

CMakeFiles/BplusTreeExe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BplusTreeExe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BplusTreeExe.dir/clean

CMakeFiles/BplusTreeExe.dir/depend:
	cd /root/xy/BplusTree/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/xy/BplusTree /root/xy/BplusTree /root/xy/BplusTree/build /root/xy/BplusTree/build /root/xy/BplusTree/build/CMakeFiles/BplusTreeExe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BplusTreeExe.dir/depend

