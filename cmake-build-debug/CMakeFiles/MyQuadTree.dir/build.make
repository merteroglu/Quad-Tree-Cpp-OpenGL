# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.6

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files (x86)\JetBrains\CLion 2016.3.3\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files (x86)\JetBrains\CLion 2016.3.3\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Sources\CLionSources\MyQuadTree

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Sources\CLionSources\MyQuadTree\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MyQuadTree.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MyQuadTree.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyQuadTree.dir/flags.make

CMakeFiles/MyQuadTree.dir/main.cpp.obj: CMakeFiles/MyQuadTree.dir/flags.make
CMakeFiles/MyQuadTree.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Sources\CLionSources\MyQuadTree\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyQuadTree.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MyQuadTree.dir\main.cpp.obj -c D:\Sources\CLionSources\MyQuadTree\main.cpp

CMakeFiles/MyQuadTree.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyQuadTree.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Sources\CLionSources\MyQuadTree\main.cpp > CMakeFiles\MyQuadTree.dir\main.cpp.i

CMakeFiles/MyQuadTree.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyQuadTree.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Sources\CLionSources\MyQuadTree\main.cpp -o CMakeFiles\MyQuadTree.dir\main.cpp.s

CMakeFiles/MyQuadTree.dir/main.cpp.obj.requires:

.PHONY : CMakeFiles/MyQuadTree.dir/main.cpp.obj.requires

CMakeFiles/MyQuadTree.dir/main.cpp.obj.provides: CMakeFiles/MyQuadTree.dir/main.cpp.obj.requires
	$(MAKE) -f CMakeFiles\MyQuadTree.dir\build.make CMakeFiles/MyQuadTree.dir/main.cpp.obj.provides.build
.PHONY : CMakeFiles/MyQuadTree.dir/main.cpp.obj.provides

CMakeFiles/MyQuadTree.dir/main.cpp.obj.provides.build: CMakeFiles/MyQuadTree.dir/main.cpp.obj


CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj: CMakeFiles/MyQuadTree.dir/flags.make
CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj: ../QuadTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Sources\CLionSources\MyQuadTree\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj"
	C:\MinGW\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MyQuadTree.dir\QuadTree.cpp.obj -c D:\Sources\CLionSources\MyQuadTree\QuadTree.cpp

CMakeFiles/MyQuadTree.dir/QuadTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyQuadTree.dir/QuadTree.cpp.i"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Sources\CLionSources\MyQuadTree\QuadTree.cpp > CMakeFiles\MyQuadTree.dir\QuadTree.cpp.i

CMakeFiles/MyQuadTree.dir/QuadTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyQuadTree.dir/QuadTree.cpp.s"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Sources\CLionSources\MyQuadTree\QuadTree.cpp -o CMakeFiles\MyQuadTree.dir\QuadTree.cpp.s

CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj.requires:

.PHONY : CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj.requires

CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj.provides: CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj.requires
	$(MAKE) -f CMakeFiles\MyQuadTree.dir\build.make CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj.provides.build
.PHONY : CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj.provides

CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj.provides.build: CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj


# Object files for target MyQuadTree
MyQuadTree_OBJECTS = \
"CMakeFiles/MyQuadTree.dir/main.cpp.obj" \
"CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj"

# External object files for target MyQuadTree
MyQuadTree_EXTERNAL_OBJECTS =

MyQuadTree.exe: CMakeFiles/MyQuadTree.dir/main.cpp.obj
MyQuadTree.exe: CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj
MyQuadTree.exe: CMakeFiles/MyQuadTree.dir/build.make
MyQuadTree.exe: CMakeFiles/MyQuadTree.dir/linklibs.rsp
MyQuadTree.exe: CMakeFiles/MyQuadTree.dir/objects1.rsp
MyQuadTree.exe: CMakeFiles/MyQuadTree.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Sources\CLionSources\MyQuadTree\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable MyQuadTree.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\MyQuadTree.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyQuadTree.dir/build: MyQuadTree.exe

.PHONY : CMakeFiles/MyQuadTree.dir/build

CMakeFiles/MyQuadTree.dir/requires: CMakeFiles/MyQuadTree.dir/main.cpp.obj.requires
CMakeFiles/MyQuadTree.dir/requires: CMakeFiles/MyQuadTree.dir/QuadTree.cpp.obj.requires

.PHONY : CMakeFiles/MyQuadTree.dir/requires

CMakeFiles/MyQuadTree.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\MyQuadTree.dir\cmake_clean.cmake
.PHONY : CMakeFiles/MyQuadTree.dir/clean

CMakeFiles/MyQuadTree.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Sources\CLionSources\MyQuadTree D:\Sources\CLionSources\MyQuadTree D:\Sources\CLionSources\MyQuadTree\cmake-build-debug D:\Sources\CLionSources\MyQuadTree\cmake-build-debug D:\Sources\CLionSources\MyQuadTree\cmake-build-debug\CMakeFiles\MyQuadTree.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MyQuadTree.dir/depend

