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
include curl/tests/libtest/CMakeFiles/lib2404.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include curl/tests/libtest/CMakeFiles/lib2404.dir/compiler_depend.make

# Include the progress variables for this target.
include curl/tests/libtest/CMakeFiles/lib2404.dir/progress.make

# Include the compile flags for this target's objects.
include curl/tests/libtest/CMakeFiles/lib2404.dir/flags.make

curl/tests/libtest/CMakeFiles/lib2404.dir/lib2404.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/flags.make
curl/tests/libtest/CMakeFiles/lib2404.dir/lib2404.c.o: ../curl/tests/libtest/lib2404.c
curl/tests/libtest/CMakeFiles/lib2404.dir/lib2404.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object curl/tests/libtest/CMakeFiles/lib2404.dir/lib2404.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/libtest/CMakeFiles/lib2404.dir/lib2404.c.o -MF CMakeFiles/lib2404.dir/lib2404.c.o.d -o CMakeFiles/lib2404.dir/lib2404.c.o -c /home/bonson/slow5-wget/curl/tests/libtest/lib2404.c

curl/tests/libtest/CMakeFiles/lib2404.dir/lib2404.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib2404.dir/lib2404.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/tests/libtest/lib2404.c > CMakeFiles/lib2404.dir/lib2404.c.i

curl/tests/libtest/CMakeFiles/lib2404.dir/lib2404.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib2404.dir/lib2404.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/tests/libtest/lib2404.c -o CMakeFiles/lib2404.dir/lib2404.c.s

curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/timediff.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/flags.make
curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/timediff.c.o: ../curl/lib/timediff.c
curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/timediff.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/timediff.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/timediff.c.o -MF CMakeFiles/lib2404.dir/__/__/lib/timediff.c.o.d -o CMakeFiles/lib2404.dir/__/__/lib/timediff.c.o -c /home/bonson/slow5-wget/curl/lib/timediff.c

curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/timediff.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib2404.dir/__/__/lib/timediff.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/timediff.c > CMakeFiles/lib2404.dir/__/__/lib/timediff.c.i

curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/timediff.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib2404.dir/__/__/lib/timediff.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/timediff.c -o CMakeFiles/lib2404.dir/__/__/lib/timediff.c.s

curl/tests/libtest/CMakeFiles/lib2404.dir/first.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/flags.make
curl/tests/libtest/CMakeFiles/lib2404.dir/first.c.o: ../curl/tests/libtest/first.c
curl/tests/libtest/CMakeFiles/lib2404.dir/first.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object curl/tests/libtest/CMakeFiles/lib2404.dir/first.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/libtest/CMakeFiles/lib2404.dir/first.c.o -MF CMakeFiles/lib2404.dir/first.c.o.d -o CMakeFiles/lib2404.dir/first.c.o -c /home/bonson/slow5-wget/curl/tests/libtest/first.c

curl/tests/libtest/CMakeFiles/lib2404.dir/first.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib2404.dir/first.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/tests/libtest/first.c > CMakeFiles/lib2404.dir/first.c.i

curl/tests/libtest/CMakeFiles/lib2404.dir/first.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib2404.dir/first.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/tests/libtest/first.c -o CMakeFiles/lib2404.dir/first.c.s

curl/tests/libtest/CMakeFiles/lib2404.dir/testutil.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/flags.make
curl/tests/libtest/CMakeFiles/lib2404.dir/testutil.c.o: ../curl/tests/libtest/testutil.c
curl/tests/libtest/CMakeFiles/lib2404.dir/testutil.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object curl/tests/libtest/CMakeFiles/lib2404.dir/testutil.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/libtest/CMakeFiles/lib2404.dir/testutil.c.o -MF CMakeFiles/lib2404.dir/testutil.c.o.d -o CMakeFiles/lib2404.dir/testutil.c.o -c /home/bonson/slow5-wget/curl/tests/libtest/testutil.c

curl/tests/libtest/CMakeFiles/lib2404.dir/testutil.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib2404.dir/testutil.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/tests/libtest/testutil.c > CMakeFiles/lib2404.dir/testutil.c.i

curl/tests/libtest/CMakeFiles/lib2404.dir/testutil.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib2404.dir/testutil.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/tests/libtest/testutil.c -o CMakeFiles/lib2404.dir/testutil.c.s

curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/warnless.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/flags.make
curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/warnless.c.o: ../curl/lib/warnless.c
curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/warnless.c.o: curl/tests/libtest/CMakeFiles/lib2404.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/warnless.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/warnless.c.o -MF CMakeFiles/lib2404.dir/__/__/lib/warnless.c.o.d -o CMakeFiles/lib2404.dir/__/__/lib/warnless.c.o -c /home/bonson/slow5-wget/curl/lib/warnless.c

curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/warnless.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib2404.dir/__/__/lib/warnless.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/warnless.c > CMakeFiles/lib2404.dir/__/__/lib/warnless.c.i

curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/warnless.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib2404.dir/__/__/lib/warnless.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/warnless.c -o CMakeFiles/lib2404.dir/__/__/lib/warnless.c.s

# Object files for target lib2404
lib2404_OBJECTS = \
"CMakeFiles/lib2404.dir/lib2404.c.o" \
"CMakeFiles/lib2404.dir/__/__/lib/timediff.c.o" \
"CMakeFiles/lib2404.dir/first.c.o" \
"CMakeFiles/lib2404.dir/testutil.c.o" \
"CMakeFiles/lib2404.dir/__/__/lib/warnless.c.o"

# External object files for target lib2404
lib2404_EXTERNAL_OBJECTS =

curl/tests/libtest/lib2404: curl/tests/libtest/CMakeFiles/lib2404.dir/lib2404.c.o
curl/tests/libtest/lib2404: curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/timediff.c.o
curl/tests/libtest/lib2404: curl/tests/libtest/CMakeFiles/lib2404.dir/first.c.o
curl/tests/libtest/lib2404: curl/tests/libtest/CMakeFiles/lib2404.dir/testutil.c.o
curl/tests/libtest/lib2404: curl/tests/libtest/CMakeFiles/lib2404.dir/__/__/lib/warnless.c.o
curl/tests/libtest/lib2404: curl/tests/libtest/CMakeFiles/lib2404.dir/build.make
curl/tests/libtest/lib2404: curl/lib/libcurl.so.4.8.0
curl/tests/libtest/lib2404: /usr/lib/x86_64-linux-gnu/libssl.so
curl/tests/libtest/lib2404: /usr/lib/x86_64-linux-gnu/libcrypto.so
curl/tests/libtest/lib2404: /usr/lib/x86_64-linux-gnu/libz.so
curl/tests/libtest/lib2404: curl/tests/libtest/CMakeFiles/lib2404.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable lib2404"
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lib2404.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
curl/tests/libtest/CMakeFiles/lib2404.dir/build: curl/tests/libtest/lib2404
.PHONY : curl/tests/libtest/CMakeFiles/lib2404.dir/build

curl/tests/libtest/CMakeFiles/lib2404.dir/clean:
	cd /home/bonson/slow5-wget/build/curl/tests/libtest && $(CMAKE_COMMAND) -P CMakeFiles/lib2404.dir/cmake_clean.cmake
.PHONY : curl/tests/libtest/CMakeFiles/lib2404.dir/clean

curl/tests/libtest/CMakeFiles/lib2404.dir/depend:
	cd /home/bonson/slow5-wget/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bonson/slow5-wget /home/bonson/slow5-wget/curl/tests/libtest /home/bonson/slow5-wget/build /home/bonson/slow5-wget/build/curl/tests/libtest /home/bonson/slow5-wget/build/curl/tests/libtest/CMakeFiles/lib2404.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : curl/tests/libtest/CMakeFiles/lib2404.dir/depend

