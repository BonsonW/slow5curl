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

# Include any dependencies generated for this target.
include slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/compiler_depend.make

# Include the progress variables for this target.
include slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/progress.make

# Include the compile flags for this target's objects.
include slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/flags.make

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.o: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/flags.make
slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.o: ../slow5lib/thirdparty/streamvbyte/src/streamvbyte_decode.c
slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.o: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.o"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.o -MF CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.o.d -o CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.o -c /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte/src/streamvbyte_decode.c

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.i"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte/src/streamvbyte_decode.c > CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.i

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.s"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte/src/streamvbyte_decode.c -o CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.s

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.o: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/flags.make
slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.o: ../slow5lib/thirdparty/streamvbyte/src/streamvbyte_encode.c
slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.o: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.o"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.o -MF CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.o.d -o CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.o -c /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte/src/streamvbyte_encode.c

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.i"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte/src/streamvbyte_encode.c > CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.i

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.s"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte/src/streamvbyte_encode.c -o CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.s

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.o: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/flags.make
slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.o: ../slow5lib/thirdparty/streamvbyte/src/streamvbyte_zigzag.c
slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.o: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.o"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.o -MF CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.o.d -o CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.o -c /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte/src/streamvbyte_zigzag.c

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.i"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte/src/streamvbyte_zigzag.c > CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.i

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.s"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte/src/streamvbyte_zigzag.c -o CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.s

# Object files for target streamvbyte_slow5
streamvbyte_slow5_OBJECTS = \
"CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.o" \
"CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.o" \
"CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.o"

# External object files for target streamvbyte_slow5
streamvbyte_slow5_EXTERNAL_OBJECTS =

slow5lib/thirdparty/streamvbyte/libstreamvbyte_slow5.a: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_decode.c.o
slow5lib/thirdparty/streamvbyte/libstreamvbyte_slow5.a: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_encode.c.o
slow5lib/thirdparty/streamvbyte/libstreamvbyte_slow5.a: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/src/streamvbyte_zigzag.c.o
slow5lib/thirdparty/streamvbyte/libstreamvbyte_slow5.a: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/build.make
slow5lib/thirdparty/streamvbyte/libstreamvbyte_slow5.a: slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C static library libstreamvbyte_slow5.a"
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && $(CMAKE_COMMAND) -P CMakeFiles/streamvbyte_slow5.dir/cmake_clean_target.cmake
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/streamvbyte_slow5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/build: slow5lib/thirdparty/streamvbyte/libstreamvbyte_slow5.a
.PHONY : slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/build

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/clean:
	cd /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte && $(CMAKE_COMMAND) -P CMakeFiles/streamvbyte_slow5.dir/cmake_clean.cmake
.PHONY : slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/clean

slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/depend:
	cd /home/bonson/slow5-wget/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bonson/slow5-wget /home/bonson/slow5-wget/slow5lib/thirdparty/streamvbyte /home/bonson/slow5-wget/build /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte /home/bonson/slow5-wget/build/slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : slow5lib/thirdparty/streamvbyte/CMakeFiles/streamvbyte_slow5.dir/depend
