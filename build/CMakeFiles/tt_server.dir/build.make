# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /home/tt/ttWebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tt/ttWebServer/build

# Include any dependencies generated for this target.
include CMakeFiles/tt_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tt_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tt_server.dir/flags.make

CMakeFiles/tt_server.dir/main.cc.o: CMakeFiles/tt_server.dir/flags.make
CMakeFiles/tt_server.dir/main.cc.o: ../main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tt_server.dir/main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tt_server.dir/main.cc.o -c /home/tt/ttWebServer/main.cc

CMakeFiles/tt_server.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tt_server.dir/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/main.cc > CMakeFiles/tt_server.dir/main.cc.i

CMakeFiles/tt_server.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tt_server.dir/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/main.cc -o CMakeFiles/tt_server.dir/main.cc.s

# Object files for target tt_server
tt_server_OBJECTS = \
"CMakeFiles/tt_server.dir/main.cc.o"

# External object files for target tt_server
tt_server_EXTERNAL_OBJECTS =

tt_server: CMakeFiles/tt_server.dir/main.cc.o
tt_server: CMakeFiles/tt_server.dir/build.make
tt_server: net/libNetFunctions.a
tt_server: base/libBaseFunctions.a
tt_server: http/libHttpFunctions.a
tt_server: CMakeFiles/tt_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tt/ttWebServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tt_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tt_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tt_server.dir/build: tt_server

.PHONY : CMakeFiles/tt_server.dir/build

CMakeFiles/tt_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tt_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tt_server.dir/clean

CMakeFiles/tt_server.dir/depend:
	cd /home/tt/ttWebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tt/ttWebServer /home/tt/ttWebServer /home/tt/ttWebServer/build /home/tt/ttWebServer/build /home/tt/ttWebServer/build/CMakeFiles/tt_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tt_server.dir/depend

