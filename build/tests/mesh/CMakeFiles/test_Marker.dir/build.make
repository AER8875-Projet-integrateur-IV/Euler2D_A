# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/olivier/Documents/PI4/Euler2D_A

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/olivier/Documents/PI4/Euler2D_A/build

# Include any dependencies generated for this target.
include tests/mesh/CMakeFiles/test_Marker.dir/depend.make

# Include the progress variables for this target.
include tests/mesh/CMakeFiles/test_Marker.dir/progress.make

# Include the compile flags for this target's objects.
include tests/mesh/CMakeFiles/test_Marker.dir/flags.make

tests/mesh/CMakeFiles/test_Marker.dir/test_Marker.cpp.o: tests/mesh/CMakeFiles/test_Marker.dir/flags.make
tests/mesh/CMakeFiles/test_Marker.dir/test_Marker.cpp.o: ../tests/mesh/test_Marker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/olivier/Documents/PI4/Euler2D_A/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/mesh/CMakeFiles/test_Marker.dir/test_Marker.cpp.o"
	cd /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_Marker.dir/test_Marker.cpp.o -c /home/olivier/Documents/PI4/Euler2D_A/tests/mesh/test_Marker.cpp

tests/mesh/CMakeFiles/test_Marker.dir/test_Marker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_Marker.dir/test_Marker.cpp.i"
	cd /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/olivier/Documents/PI4/Euler2D_A/tests/mesh/test_Marker.cpp > CMakeFiles/test_Marker.dir/test_Marker.cpp.i

tests/mesh/CMakeFiles/test_Marker.dir/test_Marker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_Marker.dir/test_Marker.cpp.s"
	cd /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/olivier/Documents/PI4/Euler2D_A/tests/mesh/test_Marker.cpp -o CMakeFiles/test_Marker.dir/test_Marker.cpp.s

# Object files for target test_Marker
test_Marker_OBJECTS = \
"CMakeFiles/test_Marker.dir/test_Marker.cpp.o"

# External object files for target test_Marker
test_Marker_EXTERNAL_OBJECTS =

tests/mesh/test_Marker: tests/mesh/CMakeFiles/test_Marker.dir/test_Marker.cpp.o
tests/mesh/test_Marker: tests/mesh/CMakeFiles/test_Marker.dir/build.make
tests/mesh/test_Marker: lib/libgtestd.a
tests/mesh/test_Marker: lib/libgmockd.a
tests/mesh/test_Marker: lib/libgtest_maind.a
tests/mesh/test_Marker: src/mesh/libMesh.a
tests/mesh/test_Marker: lib/libgtestd.a
tests/mesh/test_Marker: src/utils/logger/libLogger.a
tests/mesh/test_Marker: tests/mesh/CMakeFiles/test_Marker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/olivier/Documents/PI4/Euler2D_A/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_Marker"
	cd /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_Marker.dir/link.txt --verbose=$(VERBOSE)
	cd /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh && /usr/bin/cmake -D TEST_TARGET=test_Marker -D TEST_EXECUTABLE=/home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/test_Marker -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES=VS_DEBUGGER_WORKING_DIRECTORY -D TEST_PREFIX= -D TEST_SUFFIX= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=test_Marker_TESTS -D CTEST_FILE=/home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/test_Marker[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -P /usr/share/cmake-3.16/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
tests/mesh/CMakeFiles/test_Marker.dir/build: tests/mesh/test_Marker

.PHONY : tests/mesh/CMakeFiles/test_Marker.dir/build

tests/mesh/CMakeFiles/test_Marker.dir/clean:
	cd /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh && $(CMAKE_COMMAND) -P CMakeFiles/test_Marker.dir/cmake_clean.cmake
.PHONY : tests/mesh/CMakeFiles/test_Marker.dir/clean

tests/mesh/CMakeFiles/test_Marker.dir/depend:
	cd /home/olivier/Documents/PI4/Euler2D_A/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/olivier/Documents/PI4/Euler2D_A /home/olivier/Documents/PI4/Euler2D_A/tests/mesh /home/olivier/Documents/PI4/Euler2D_A/build /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh /home/olivier/Documents/PI4/Euler2D_A/build/tests/mesh/CMakeFiles/test_Marker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/mesh/CMakeFiles/test_Marker.dir/depend

