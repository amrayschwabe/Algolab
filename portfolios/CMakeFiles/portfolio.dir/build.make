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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.12.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.12.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios"

# Include any dependencies generated for this target.
include CMakeFiles/portfolio.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/portfolio.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/portfolio.dir/flags.make

CMakeFiles/portfolio.dir/portfolio.cpp.o: CMakeFiles/portfolio.dir/flags.make
CMakeFiles/portfolio.dir/portfolio.cpp.o: portfolio.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/portfolio.dir/portfolio.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/portfolio.dir/portfolio.cpp.o -c "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios/portfolio.cpp"

CMakeFiles/portfolio.dir/portfolio.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/portfolio.dir/portfolio.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios/portfolio.cpp" > CMakeFiles/portfolio.dir/portfolio.cpp.i

CMakeFiles/portfolio.dir/portfolio.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/portfolio.dir/portfolio.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios/portfolio.cpp" -o CMakeFiles/portfolio.dir/portfolio.cpp.s

# Object files for target portfolio
portfolio_OBJECTS = \
"CMakeFiles/portfolio.dir/portfolio.cpp.o"

# External object files for target portfolio
portfolio_EXTERNAL_OBJECTS =

portfolio: CMakeFiles/portfolio.dir/portfolio.cpp.o
portfolio: CMakeFiles/portfolio.dir/build.make
portfolio: /usr/local/lib/libmpfr.dylib
portfolio: /usr/local/lib/libgmp.dylib
portfolio: /usr/local/lib/libCGAL_Core.13.0.2.dylib
portfolio: /usr/local/lib/libboost_thread-mt.dylib
portfolio: /usr/local/lib/libboost_system-mt.dylib
portfolio: /usr/local/lib/libboost_chrono-mt.dylib
portfolio: /usr/local/lib/libboost_date_time-mt.dylib
portfolio: /usr/local/lib/libboost_atomic-mt.dylib
portfolio: /usr/local/lib/libCGAL.13.0.2.dylib
portfolio: /usr/local/lib/libboost_thread-mt.dylib
portfolio: /usr/local/lib/libboost_system-mt.dylib
portfolio: /usr/local/lib/libboost_chrono-mt.dylib
portfolio: /usr/local/lib/libboost_date_time-mt.dylib
portfolio: /usr/local/lib/libboost_atomic-mt.dylib
portfolio: /usr/local/lib/libmpfr.dylib
portfolio: /usr/local/lib/libgmp.dylib
portfolio: /usr/local/lib/libboost_thread-mt.dylib
portfolio: /usr/local/lib/libboost_chrono-mt.dylib
portfolio: /usr/local/lib/libboost_system-mt.dylib
portfolio: /usr/local/lib/libboost_date_time-mt.dylib
portfolio: /usr/local/lib/libboost_atomic-mt.dylib
portfolio: CMakeFiles/portfolio.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable portfolio"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/portfolio.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/portfolio.dir/build: portfolio

.PHONY : CMakeFiles/portfolio.dir/build

CMakeFiles/portfolio.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/portfolio.dir/cmake_clean.cmake
.PHONY : CMakeFiles/portfolio.dir/clean

CMakeFiles/portfolio.dir/depend:
	cd "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/portfolios/CMakeFiles/portfolio.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/portfolio.dir/depend

