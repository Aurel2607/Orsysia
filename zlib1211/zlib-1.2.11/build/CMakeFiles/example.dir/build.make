# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11\build

# Include any dependencies generated for this target.
include CMakeFiles/example.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/example.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/example.dir/flags.make

CMakeFiles/example.dir/test/example.obj: CMakeFiles/example.dir/flags.make
CMakeFiles/example.dir/test/example.obj: CMakeFiles/example.dir/includes_C.rsp
CMakeFiles/example.dir/test/example.obj: ../test/example.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/example.dir/test/example.obj"
	C:\PROGRA~2\CODEBL~1\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\example.dir\test\example.obj   -c E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11\test\example.c

CMakeFiles/example.dir/test/example.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/example.dir/test/example.i"
	C:\PROGRA~2\CODEBL~1\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11\test\example.c > CMakeFiles\example.dir\test\example.i

CMakeFiles/example.dir/test/example.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/example.dir/test/example.s"
	C:\PROGRA~2\CODEBL~1\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11\test\example.c -o CMakeFiles\example.dir\test\example.s

# Object files for target example
example_OBJECTS = \
"CMakeFiles/example.dir/test/example.obj"

# External object files for target example
example_EXTERNAL_OBJECTS =

example.exe: CMakeFiles/example.dir/test/example.obj
example.exe: CMakeFiles/example.dir/build.make
example.exe: libzlib.dll.a
example.exe: CMakeFiles/example.dir/linklibs.rsp
example.exe: CMakeFiles/example.dir/objects1.rsp
example.exe: CMakeFiles/example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable example.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\example.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/example.dir/build: example.exe

.PHONY : CMakeFiles/example.dir/build

CMakeFiles/example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\example.dir\cmake_clean.cmake
.PHONY : CMakeFiles/example.dir/clean

CMakeFiles/example.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11 E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11 E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11\build E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11\build E:\Perso\DevPerso\DevGame\zlib1211\zlib-1.2.11\build\CMakeFiles\example.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/example.dir/depend

