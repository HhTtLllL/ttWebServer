# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_SOURCE_DIR = /home/tt/ttWebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tt/ttWebServer

# Include any dependencies generated for this target.
include base/CMakeFiles/BaseFunctions.dir/depend.make

# Include the progress variables for this target.
include base/CMakeFiles/BaseFunctions.dir/progress.make

# Include the compile flags for this target's objects.
include base/CMakeFiles/BaseFunctions.dir/flags.make

base/CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.o: base/CMakeFiles/BaseFunctions.dir/flags.make
base/CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.o: base/AsyncLogging.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object base/CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.o"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.o -c /home/tt/ttWebServer/base/AsyncLogging.cc

base/CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.i"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/base/AsyncLogging.cc > CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.i

base/CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.s"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/base/AsyncLogging.cc -o CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.s

base/CMakeFiles/BaseFunctions.dir/Condition.cc.o: base/CMakeFiles/BaseFunctions.dir/flags.make
base/CMakeFiles/BaseFunctions.dir/Condition.cc.o: base/Condition.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object base/CMakeFiles/BaseFunctions.dir/Condition.cc.o"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BaseFunctions.dir/Condition.cc.o -c /home/tt/ttWebServer/base/Condition.cc

base/CMakeFiles/BaseFunctions.dir/Condition.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BaseFunctions.dir/Condition.cc.i"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/base/Condition.cc > CMakeFiles/BaseFunctions.dir/Condition.cc.i

base/CMakeFiles/BaseFunctions.dir/Condition.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BaseFunctions.dir/Condition.cc.s"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/base/Condition.cc -o CMakeFiles/BaseFunctions.dir/Condition.cc.s

base/CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.o: base/CMakeFiles/BaseFunctions.dir/flags.make
base/CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.o: base/CountDownLatch.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object base/CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.o"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.o -c /home/tt/ttWebServer/base/CountDownLatch.cc

base/CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.i"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/base/CountDownLatch.cc > CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.i

base/CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.s"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/base/CountDownLatch.cc -o CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.s

base/CMakeFiles/BaseFunctions.dir/CurrentThread.cc.o: base/CMakeFiles/BaseFunctions.dir/flags.make
base/CMakeFiles/BaseFunctions.dir/CurrentThread.cc.o: base/CurrentThread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object base/CMakeFiles/BaseFunctions.dir/CurrentThread.cc.o"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BaseFunctions.dir/CurrentThread.cc.o -c /home/tt/ttWebServer/base/CurrentThread.cc

base/CMakeFiles/BaseFunctions.dir/CurrentThread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BaseFunctions.dir/CurrentThread.cc.i"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/base/CurrentThread.cc > CMakeFiles/BaseFunctions.dir/CurrentThread.cc.i

base/CMakeFiles/BaseFunctions.dir/CurrentThread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BaseFunctions.dir/CurrentThread.cc.s"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/base/CurrentThread.cc -o CMakeFiles/BaseFunctions.dir/CurrentThread.cc.s

base/CMakeFiles/BaseFunctions.dir/FileUtil.cc.o: base/CMakeFiles/BaseFunctions.dir/flags.make
base/CMakeFiles/BaseFunctions.dir/FileUtil.cc.o: base/FileUtil.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object base/CMakeFiles/BaseFunctions.dir/FileUtil.cc.o"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BaseFunctions.dir/FileUtil.cc.o -c /home/tt/ttWebServer/base/FileUtil.cc

base/CMakeFiles/BaseFunctions.dir/FileUtil.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BaseFunctions.dir/FileUtil.cc.i"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/base/FileUtil.cc > CMakeFiles/BaseFunctions.dir/FileUtil.cc.i

base/CMakeFiles/BaseFunctions.dir/FileUtil.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BaseFunctions.dir/FileUtil.cc.s"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/base/FileUtil.cc -o CMakeFiles/BaseFunctions.dir/FileUtil.cc.s

base/CMakeFiles/BaseFunctions.dir/LogFile.cc.o: base/CMakeFiles/BaseFunctions.dir/flags.make
base/CMakeFiles/BaseFunctions.dir/LogFile.cc.o: base/LogFile.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object base/CMakeFiles/BaseFunctions.dir/LogFile.cc.o"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BaseFunctions.dir/LogFile.cc.o -c /home/tt/ttWebServer/base/LogFile.cc

base/CMakeFiles/BaseFunctions.dir/LogFile.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BaseFunctions.dir/LogFile.cc.i"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/base/LogFile.cc > CMakeFiles/BaseFunctions.dir/LogFile.cc.i

base/CMakeFiles/BaseFunctions.dir/LogFile.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BaseFunctions.dir/LogFile.cc.s"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/base/LogFile.cc -o CMakeFiles/BaseFunctions.dir/LogFile.cc.s

base/CMakeFiles/BaseFunctions.dir/LogStream.cc.o: base/CMakeFiles/BaseFunctions.dir/flags.make
base/CMakeFiles/BaseFunctions.dir/LogStream.cc.o: base/LogStream.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object base/CMakeFiles/BaseFunctions.dir/LogStream.cc.o"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BaseFunctions.dir/LogStream.cc.o -c /home/tt/ttWebServer/base/LogStream.cc

base/CMakeFiles/BaseFunctions.dir/LogStream.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BaseFunctions.dir/LogStream.cc.i"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/base/LogStream.cc > CMakeFiles/BaseFunctions.dir/LogStream.cc.i

base/CMakeFiles/BaseFunctions.dir/LogStream.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BaseFunctions.dir/LogStream.cc.s"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/base/LogStream.cc -o CMakeFiles/BaseFunctions.dir/LogStream.cc.s

base/CMakeFiles/BaseFunctions.dir/Logging.cc.o: base/CMakeFiles/BaseFunctions.dir/flags.make
base/CMakeFiles/BaseFunctions.dir/Logging.cc.o: base/Logging.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object base/CMakeFiles/BaseFunctions.dir/Logging.cc.o"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BaseFunctions.dir/Logging.cc.o -c /home/tt/ttWebServer/base/Logging.cc

base/CMakeFiles/BaseFunctions.dir/Logging.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BaseFunctions.dir/Logging.cc.i"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/base/Logging.cc > CMakeFiles/BaseFunctions.dir/Logging.cc.i

base/CMakeFiles/BaseFunctions.dir/Logging.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BaseFunctions.dir/Logging.cc.s"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/base/Logging.cc -o CMakeFiles/BaseFunctions.dir/Logging.cc.s

base/CMakeFiles/BaseFunctions.dir/Thread.cc.o: base/CMakeFiles/BaseFunctions.dir/flags.make
base/CMakeFiles/BaseFunctions.dir/Thread.cc.o: base/Thread.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object base/CMakeFiles/BaseFunctions.dir/Thread.cc.o"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BaseFunctions.dir/Thread.cc.o -c /home/tt/ttWebServer/base/Thread.cc

base/CMakeFiles/BaseFunctions.dir/Thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BaseFunctions.dir/Thread.cc.i"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tt/ttWebServer/base/Thread.cc > CMakeFiles/BaseFunctions.dir/Thread.cc.i

base/CMakeFiles/BaseFunctions.dir/Thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BaseFunctions.dir/Thread.cc.s"
	cd /home/tt/ttWebServer/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tt/ttWebServer/base/Thread.cc -o CMakeFiles/BaseFunctions.dir/Thread.cc.s

# Object files for target BaseFunctions
BaseFunctions_OBJECTS = \
"CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.o" \
"CMakeFiles/BaseFunctions.dir/Condition.cc.o" \
"CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.o" \
"CMakeFiles/BaseFunctions.dir/CurrentThread.cc.o" \
"CMakeFiles/BaseFunctions.dir/FileUtil.cc.o" \
"CMakeFiles/BaseFunctions.dir/LogFile.cc.o" \
"CMakeFiles/BaseFunctions.dir/LogStream.cc.o" \
"CMakeFiles/BaseFunctions.dir/Logging.cc.o" \
"CMakeFiles/BaseFunctions.dir/Thread.cc.o"

# External object files for target BaseFunctions
BaseFunctions_EXTERNAL_OBJECTS =

base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/AsyncLogging.cc.o
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/Condition.cc.o
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/CountDownLatch.cc.o
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/CurrentThread.cc.o
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/FileUtil.cc.o
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/LogFile.cc.o
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/LogStream.cc.o
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/Logging.cc.o
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/Thread.cc.o
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/build.make
base/libBaseFunctions.a: base/CMakeFiles/BaseFunctions.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tt/ttWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX static library libBaseFunctions.a"
	cd /home/tt/ttWebServer/base && $(CMAKE_COMMAND) -P CMakeFiles/BaseFunctions.dir/cmake_clean_target.cmake
	cd /home/tt/ttWebServer/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BaseFunctions.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
base/CMakeFiles/BaseFunctions.dir/build: base/libBaseFunctions.a

.PHONY : base/CMakeFiles/BaseFunctions.dir/build

base/CMakeFiles/BaseFunctions.dir/clean:
	cd /home/tt/ttWebServer/base && $(CMAKE_COMMAND) -P CMakeFiles/BaseFunctions.dir/cmake_clean.cmake
.PHONY : base/CMakeFiles/BaseFunctions.dir/clean

base/CMakeFiles/BaseFunctions.dir/depend:
	cd /home/tt/ttWebServer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tt/ttWebServer /home/tt/ttWebServer/base /home/tt/ttWebServer /home/tt/ttWebServer/base /home/tt/ttWebServer/base/CMakeFiles/BaseFunctions.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : base/CMakeFiles/BaseFunctions.dir/depend

