# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /cygdrive/c/Users/MissingNO/.CLion2019.2/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/MissingNO/.CLion2019.2/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/variable-register-example.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/variable-register-example.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/variable-register-example.dir/flags.make

CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.o: CMakeFiles/variable-register-example.dir/flags.make
CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.o: ../examples/vari_reg_example/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.o -c /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/examples/vari_reg_example/main.cpp

CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/examples/vari_reg_example/main.cpp > CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.i

CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/examples/vari_reg_example/main.cpp -o CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.s

# Object files for target variable-register-example
variable__register__example_OBJECTS = \
"CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.o"

# External object files for target variable-register-example
variable__register__example_EXTERNAL_OBJECTS =

variable-register-example.exe: CMakeFiles/variable-register-example.dir/examples/vari_reg_example/main.cpp.o
variable-register-example.exe: CMakeFiles/variable-register-example.dir/build.make
variable-register-example.exe: CMakeFiles/variable-register-example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable variable-register-example.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/variable-register-example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/variable-register-example.dir/build: variable-register-example.exe

.PHONY : CMakeFiles/variable-register-example.dir/build

CMakeFiles/variable-register-example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/variable-register-example.dir/cmake_clean.cmake
.PHONY : CMakeFiles/variable-register-example.dir/clean

CMakeFiles/variable-register-example.dir/depend:
	cd /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/cmake-build-debug /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/cmake-build-debug /cygdrive/c/playspace/personal/cpp/cellular-evolution-cpp/cmake-build-debug/CMakeFiles/variable-register-example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/variable-register-example.dir/depend

