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
CMAKE_COMMAND = /usr/local/lib/python3.6/dist-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /usr/local/lib/python3.6/dist-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bonson/slow5-wget

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bonson/slow5-wget/build

# Utility rule file for test-am.

# Include any custom commands dependencies for this target.
include curl/tests/CMakeFiles/test-am.dir/compiler_depend.make

# Include the progress variables for this target.
include curl/tests/CMakeFiles/test-am.dir/progress.make

curl/tests/CMakeFiles/test-am:
	cd /home/bonson/slow5-wget/build/curl/tests && /usr/bin/perl /home/bonson/slow5-wget/curl/tests/runtests.pl -a -am "\$$TFLAGS"

test-am: curl/tests/CMakeFiles/test-am
test-am: curl/tests/CMakeFiles/test-am.dir/build.make
.PHONY : test-am

# Rule to build all files generated by this target.
curl/tests/CMakeFiles/test-am.dir/build: test-am
.PHONY : curl/tests/CMakeFiles/test-am.dir/build

curl/tests/CMakeFiles/test-am.dir/clean:
	cd /home/bonson/slow5-wget/build/curl/tests && $(CMAKE_COMMAND) -P CMakeFiles/test-am.dir/cmake_clean.cmake
.PHONY : curl/tests/CMakeFiles/test-am.dir/clean

curl/tests/CMakeFiles/test-am.dir/depend:
	cd /home/bonson/slow5-wget/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bonson/slow5-wget /home/bonson/slow5-wget/curl/tests /home/bonson/slow5-wget/build /home/bonson/slow5-wget/build/curl/tests /home/bonson/slow5-wget/build/curl/tests/CMakeFiles/test-am.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : curl/tests/CMakeFiles/test-am.dir/depend

