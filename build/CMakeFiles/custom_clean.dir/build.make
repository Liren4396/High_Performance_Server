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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dlr/Downloads/High_Performance_Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dlr/Downloads/High_Performance_Server/build

# Utility rule file for custom_clean.

# Include any custom commands dependencies for this target.
include CMakeFiles/custom_clean.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/custom_clean.dir/progress.make

CMakeFiles/custom_clean:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Cleaning up the build directory"
	rm -rf client server

custom_clean: CMakeFiles/custom_clean
custom_clean: CMakeFiles/custom_clean.dir/build.make
.PHONY : custom_clean

# Rule to build all files generated by this target.
CMakeFiles/custom_clean.dir/build: custom_clean
.PHONY : CMakeFiles/custom_clean.dir/build

CMakeFiles/custom_clean.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/custom_clean.dir/cmake_clean.cmake
.PHONY : CMakeFiles/custom_clean.dir/clean

CMakeFiles/custom_clean.dir/depend:
	cd /home/dlr/Downloads/High_Performance_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dlr/Downloads/High_Performance_Server /home/dlr/Downloads/High_Performance_Server /home/dlr/Downloads/High_Performance_Server/build /home/dlr/Downloads/High_Performance_Server/build /home/dlr/Downloads/High_Performance_Server/build/CMakeFiles/custom_clean.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/custom_clean.dir/depend

