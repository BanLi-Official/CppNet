# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build

# Include any dependencies generated for this target.
include server/CMakeFiles/server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include server/CMakeFiles/server.dir/compiler_depend.make

# Include the progress variables for this target.
include server/CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include server/CMakeFiles/server.dir/flags.make

server/CMakeFiles/server.dir/server.cpp.o: server/CMakeFiles/server.dir/flags.make
server/CMakeFiles/server.dir/server.cpp.o: /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/server/server.cpp
server/CMakeFiles/server.dir/server.cpp.o: server/CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object server/CMakeFiles/server.dir/server.cpp.o"
	cd /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build/server && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT server/CMakeFiles/server.dir/server.cpp.o -MF CMakeFiles/server.dir/server.cpp.o.d -o CMakeFiles/server.dir/server.cpp.o -c /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/server/server.cpp

server/CMakeFiles/server.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/server.dir/server.cpp.i"
	cd /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build/server && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/server/server.cpp > CMakeFiles/server.dir/server.cpp.i

server/CMakeFiles/server.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/server.dir/server.cpp.s"
	cd /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build/server && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/server/server.cpp -o CMakeFiles/server.dir/server.cpp.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/server.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

/mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/output/server: server/CMakeFiles/server.dir/server.cpp.o
/mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/output/server: server/CMakeFiles/server.dir/build.make
/mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/output/server: server/CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/output/server"
	cd /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/CMakeFiles/server.dir/build: /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/output/server
.PHONY : server/CMakeFiles/server.dir/build

server/CMakeFiles/server.dir/clean:
	cd /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build/server && $(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : server/CMakeFiles/server.dir/clean

server/CMakeFiles/server.dir/depend:
	cd /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/server /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build/server /mnt/hgfs/VMshared/code/c++/NetSample_zhanbao/build/server/CMakeFiles/server.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : server/CMakeFiles/server.dir/depend

