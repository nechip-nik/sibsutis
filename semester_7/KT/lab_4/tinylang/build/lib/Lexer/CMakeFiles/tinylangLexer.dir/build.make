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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build

# Include any dependencies generated for this target.
include lib/Lexer/CMakeFiles/tinylangLexer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/Lexer/CMakeFiles/tinylangLexer.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/Lexer/CMakeFiles/tinylangLexer.dir/progress.make

# Include the compile flags for this target's objects.
include lib/Lexer/CMakeFiles/tinylangLexer.dir/flags.make

lib/Lexer/CMakeFiles/tinylangLexer.dir/Lexer.cpp.o: lib/Lexer/CMakeFiles/tinylangLexer.dir/flags.make
lib/Lexer/CMakeFiles/tinylangLexer.dir/Lexer.cpp.o: /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/lib/Lexer/Lexer.cpp
lib/Lexer/CMakeFiles/tinylangLexer.dir/Lexer.cpp.o: lib/Lexer/CMakeFiles/tinylangLexer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/Lexer/CMakeFiles/tinylangLexer.dir/Lexer.cpp.o"
	cd /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/lib/Lexer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/Lexer/CMakeFiles/tinylangLexer.dir/Lexer.cpp.o -MF CMakeFiles/tinylangLexer.dir/Lexer.cpp.o.d -o CMakeFiles/tinylangLexer.dir/Lexer.cpp.o -c /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/lib/Lexer/Lexer.cpp

lib/Lexer/CMakeFiles/tinylangLexer.dir/Lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tinylangLexer.dir/Lexer.cpp.i"
	cd /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/lib/Lexer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/lib/Lexer/Lexer.cpp > CMakeFiles/tinylangLexer.dir/Lexer.cpp.i

lib/Lexer/CMakeFiles/tinylangLexer.dir/Lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tinylangLexer.dir/Lexer.cpp.s"
	cd /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/lib/Lexer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/lib/Lexer/Lexer.cpp -o CMakeFiles/tinylangLexer.dir/Lexer.cpp.s

# Object files for target tinylangLexer
tinylangLexer_OBJECTS = \
"CMakeFiles/tinylangLexer.dir/Lexer.cpp.o"

# External object files for target tinylangLexer
tinylangLexer_EXTERNAL_OBJECTS =

lib/Lexer/libtinylangLexer.a: lib/Lexer/CMakeFiles/tinylangLexer.dir/Lexer.cpp.o
lib/Lexer/libtinylangLexer.a: lib/Lexer/CMakeFiles/tinylangLexer.dir/build.make
lib/Lexer/libtinylangLexer.a: lib/Lexer/CMakeFiles/tinylangLexer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libtinylangLexer.a"
	cd /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/lib/Lexer && $(CMAKE_COMMAND) -P CMakeFiles/tinylangLexer.dir/cmake_clean_target.cmake
	cd /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/lib/Lexer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tinylangLexer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/Lexer/CMakeFiles/tinylangLexer.dir/build: lib/Lexer/libtinylangLexer.a
.PHONY : lib/Lexer/CMakeFiles/tinylangLexer.dir/build

lib/Lexer/CMakeFiles/tinylangLexer.dir/clean:
	cd /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/lib/Lexer && $(CMAKE_COMMAND) -P CMakeFiles/tinylangLexer.dir/cmake_clean.cmake
.PHONY : lib/Lexer/CMakeFiles/tinylangLexer.dir/clean

lib/Lexer/CMakeFiles/tinylangLexer.dir/depend:
	cd /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/lib/Lexer /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/lib/Lexer /home/nikita/sibsutis/semester_7/KT/compilers-fall-lec4/tinylang/build/lib/Lexer/CMakeFiles/tinylangLexer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : lib/Lexer/CMakeFiles/tinylangLexer.dir/depend

