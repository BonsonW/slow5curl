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
include curl/tests/server/CMakeFiles/resolve.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.make

# Include the progress variables for this target.
include curl/tests/server/CMakeFiles/resolve.dir/progress.make

# Include the compile flags for this target's objects.
include curl/tests/server/CMakeFiles/resolve.dir/flags.make

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/mprintf.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/mprintf.c.o: ../curl/lib/mprintf.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/mprintf.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/mprintf.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/mprintf.c.o -MF CMakeFiles/resolve.dir/__/__/lib/mprintf.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/mprintf.c.o -c /home/bonson/slow5-wget/curl/lib/mprintf.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/mprintf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/mprintf.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/mprintf.c > CMakeFiles/resolve.dir/__/__/lib/mprintf.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/mprintf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/mprintf.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/mprintf.c -o CMakeFiles/resolve.dir/__/__/lib/mprintf.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/nonblock.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/nonblock.c.o: ../curl/lib/nonblock.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/nonblock.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/nonblock.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/nonblock.c.o -MF CMakeFiles/resolve.dir/__/__/lib/nonblock.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/nonblock.c.o -c /home/bonson/slow5-wget/curl/lib/nonblock.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/nonblock.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/nonblock.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/nonblock.c > CMakeFiles/resolve.dir/__/__/lib/nonblock.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/nonblock.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/nonblock.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/nonblock.c -o CMakeFiles/resolve.dir/__/__/lib/nonblock.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.o: ../curl/lib/strtoofft.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.o -MF CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.o -c /home/bonson/slow5-wget/curl/lib/strtoofft.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/strtoofft.c > CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/strtoofft.c -o CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/warnless.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/warnless.c.o: ../curl/lib/warnless.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/warnless.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/warnless.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/warnless.c.o -MF CMakeFiles/resolve.dir/__/__/lib/warnless.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/warnless.c.o -c /home/bonson/slow5-wget/curl/lib/warnless.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/warnless.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/warnless.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/warnless.c > CMakeFiles/resolve.dir/__/__/lib/warnless.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/warnless.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/warnless.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/warnless.c -o CMakeFiles/resolve.dir/__/__/lib/warnless.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/timediff.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/timediff.c.o: ../curl/lib/timediff.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/timediff.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/timediff.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/timediff.c.o -MF CMakeFiles/resolve.dir/__/__/lib/timediff.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/timediff.c.o -c /home/bonson/slow5-wget/curl/lib/timediff.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/timediff.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/timediff.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/timediff.c > CMakeFiles/resolve.dir/__/__/lib/timediff.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/timediff.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/timediff.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/timediff.c -o CMakeFiles/resolve.dir/__/__/lib/timediff.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.o: ../curl/lib/dynbuf.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.o -MF CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.o -c /home/bonson/slow5-wget/curl/lib/dynbuf.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/dynbuf.c > CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/dynbuf.c -o CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strdup.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strdup.c.o: ../curl/lib/strdup.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strdup.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strdup.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strdup.c.o -MF CMakeFiles/resolve.dir/__/__/lib/strdup.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/strdup.c.o -c /home/bonson/slow5-wget/curl/lib/strdup.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strdup.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/strdup.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/strdup.c > CMakeFiles/resolve.dir/__/__/lib/strdup.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strdup.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/strdup.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/strdup.c -o CMakeFiles/resolve.dir/__/__/lib/strdup.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strcase.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strcase.c.o: ../curl/lib/strcase.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strcase.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strcase.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strcase.c.o -MF CMakeFiles/resolve.dir/__/__/lib/strcase.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/strcase.c.o -c /home/bonson/slow5-wget/curl/lib/strcase.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strcase.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/strcase.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/strcase.c > CMakeFiles/resolve.dir/__/__/lib/strcase.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strcase.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/strcase.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/strcase.c -o CMakeFiles/resolve.dir/__/__/lib/strcase.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.o: ../curl/lib/curl_multibyte.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.o -MF CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.o -c /home/bonson/slow5-wget/curl/lib/curl_multibyte.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/curl_multibyte.c > CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/curl_multibyte.c -o CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.s

curl/tests/server/CMakeFiles/resolve.dir/getpart.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/getpart.c.o: ../curl/tests/server/getpart.c
curl/tests/server/CMakeFiles/resolve.dir/getpart.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object curl/tests/server/CMakeFiles/resolve.dir/getpart.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/getpart.c.o -MF CMakeFiles/resolve.dir/getpart.c.o.d -o CMakeFiles/resolve.dir/getpart.c.o -c /home/bonson/slow5-wget/curl/tests/server/getpart.c

curl/tests/server/CMakeFiles/resolve.dir/getpart.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/getpart.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/tests/server/getpart.c > CMakeFiles/resolve.dir/getpart.c.i

curl/tests/server/CMakeFiles/resolve.dir/getpart.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/getpart.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/tests/server/getpart.c -o CMakeFiles/resolve.dir/getpart.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/base64.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/base64.c.o: ../curl/lib/base64.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/base64.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/base64.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/base64.c.o -MF CMakeFiles/resolve.dir/__/__/lib/base64.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/base64.c.o -c /home/bonson/slow5-wget/curl/lib/base64.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/base64.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/base64.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/base64.c > CMakeFiles/resolve.dir/__/__/lib/base64.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/base64.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/base64.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/base64.c -o CMakeFiles/resolve.dir/__/__/lib/base64.c.s

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/memdebug.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/memdebug.c.o: ../curl/lib/memdebug.c
curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/memdebug.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/memdebug.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/memdebug.c.o -MF CMakeFiles/resolve.dir/__/__/lib/memdebug.c.o.d -o CMakeFiles/resolve.dir/__/__/lib/memdebug.c.o -c /home/bonson/slow5-wget/curl/lib/memdebug.c

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/memdebug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/__/__/lib/memdebug.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/lib/memdebug.c > CMakeFiles/resolve.dir/__/__/lib/memdebug.c.i

curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/memdebug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/__/__/lib/memdebug.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/lib/memdebug.c -o CMakeFiles/resolve.dir/__/__/lib/memdebug.c.s

curl/tests/server/CMakeFiles/resolve.dir/util.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/util.c.o: ../curl/tests/server/util.c
curl/tests/server/CMakeFiles/resolve.dir/util.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object curl/tests/server/CMakeFiles/resolve.dir/util.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/util.c.o -MF CMakeFiles/resolve.dir/util.c.o.d -o CMakeFiles/resolve.dir/util.c.o -c /home/bonson/slow5-wget/curl/tests/server/util.c

curl/tests/server/CMakeFiles/resolve.dir/util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/util.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/tests/server/util.c > CMakeFiles/resolve.dir/util.c.i

curl/tests/server/CMakeFiles/resolve.dir/util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/util.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/tests/server/util.c -o CMakeFiles/resolve.dir/util.c.s

curl/tests/server/CMakeFiles/resolve.dir/resolve.c.o: curl/tests/server/CMakeFiles/resolve.dir/flags.make
curl/tests/server/CMakeFiles/resolve.dir/resolve.c.o: ../curl/tests/server/resolve.c
curl/tests/server/CMakeFiles/resolve.dir/resolve.c.o: curl/tests/server/CMakeFiles/resolve.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object curl/tests/server/CMakeFiles/resolve.dir/resolve.c.o"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT curl/tests/server/CMakeFiles/resolve.dir/resolve.c.o -MF CMakeFiles/resolve.dir/resolve.c.o.d -o CMakeFiles/resolve.dir/resolve.c.o -c /home/bonson/slow5-wget/curl/tests/server/resolve.c

curl/tests/server/CMakeFiles/resolve.dir/resolve.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/resolve.dir/resolve.c.i"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bonson/slow5-wget/curl/tests/server/resolve.c > CMakeFiles/resolve.dir/resolve.c.i

curl/tests/server/CMakeFiles/resolve.dir/resolve.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/resolve.dir/resolve.c.s"
	cd /home/bonson/slow5-wget/build/curl/tests/server && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bonson/slow5-wget/curl/tests/server/resolve.c -o CMakeFiles/resolve.dir/resolve.c.s

# Object files for target resolve
resolve_OBJECTS = \
"CMakeFiles/resolve.dir/__/__/lib/mprintf.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/nonblock.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/warnless.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/timediff.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/strdup.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/strcase.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.o" \
"CMakeFiles/resolve.dir/getpart.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/base64.c.o" \
"CMakeFiles/resolve.dir/__/__/lib/memdebug.c.o" \
"CMakeFiles/resolve.dir/util.c.o" \
"CMakeFiles/resolve.dir/resolve.c.o"

# External object files for target resolve
resolve_EXTERNAL_OBJECTS =

curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/mprintf.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/nonblock.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strtoofft.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/warnless.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/timediff.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/dynbuf.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strdup.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/strcase.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/curl_multibyte.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/getpart.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/base64.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/__/__/lib/memdebug.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/util.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/resolve.c.o
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/build.make
curl/tests/server/resolve: /usr/lib/x86_64-linux-gnu/libssl.so
curl/tests/server/resolve: /usr/lib/x86_64-linux-gnu/libcrypto.so
curl/tests/server/resolve: /usr/lib/x86_64-linux-gnu/libz.so
curl/tests/server/resolve: curl/tests/server/CMakeFiles/resolve.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bonson/slow5-wget/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking C executable resolve"
	cd /home/bonson/slow5-wget/build/curl/tests/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/resolve.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
curl/tests/server/CMakeFiles/resolve.dir/build: curl/tests/server/resolve
.PHONY : curl/tests/server/CMakeFiles/resolve.dir/build

curl/tests/server/CMakeFiles/resolve.dir/clean:
	cd /home/bonson/slow5-wget/build/curl/tests/server && $(CMAKE_COMMAND) -P CMakeFiles/resolve.dir/cmake_clean.cmake
.PHONY : curl/tests/server/CMakeFiles/resolve.dir/clean

curl/tests/server/CMakeFiles/resolve.dir/depend:
	cd /home/bonson/slow5-wget/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bonson/slow5-wget /home/bonson/slow5-wget/curl/tests/server /home/bonson/slow5-wget/build /home/bonson/slow5-wget/build/curl/tests/server /home/bonson/slow5-wget/build/curl/tests/server/CMakeFiles/resolve.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : curl/tests/server/CMakeFiles/resolve.dir/depend

