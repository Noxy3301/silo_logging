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
CMAKE_SOURCE_DIR = /home/noxy/cpp/silo_logging

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/noxy/cpp/silo_logging/build

# Include any dependencies generated for this target.
include CMakeFiles/silo.exe.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/silo.exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/silo.exe.dir/flags.make

CMakeFiles/silo.exe.dir/log_buffer.cpp.o: CMakeFiles/silo.exe.dir/flags.make
CMakeFiles/silo.exe.dir/log_buffer.cpp.o: ../log_buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/noxy/cpp/silo_logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/silo.exe.dir/log_buffer.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/silo.exe.dir/log_buffer.cpp.o -c /home/noxy/cpp/silo_logging/log_buffer.cpp

CMakeFiles/silo.exe.dir/log_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/silo.exe.dir/log_buffer.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/noxy/cpp/silo_logging/log_buffer.cpp > CMakeFiles/silo.exe.dir/log_buffer.cpp.i

CMakeFiles/silo.exe.dir/log_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/silo.exe.dir/log_buffer.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/noxy/cpp/silo_logging/log_buffer.cpp -o CMakeFiles/silo.exe.dir/log_buffer.cpp.s

CMakeFiles/silo.exe.dir/logger.cpp.o: CMakeFiles/silo.exe.dir/flags.make
CMakeFiles/silo.exe.dir/logger.cpp.o: ../logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/noxy/cpp/silo_logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/silo.exe.dir/logger.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/silo.exe.dir/logger.cpp.o -c /home/noxy/cpp/silo_logging/logger.cpp

CMakeFiles/silo.exe.dir/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/silo.exe.dir/logger.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/noxy/cpp/silo_logging/logger.cpp > CMakeFiles/silo.exe.dir/logger.cpp.i

CMakeFiles/silo.exe.dir/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/silo.exe.dir/logger.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/noxy/cpp/silo_logging/logger.cpp -o CMakeFiles/silo.exe.dir/logger.cpp.s

CMakeFiles/silo.exe.dir/main.cpp.o: CMakeFiles/silo.exe.dir/flags.make
CMakeFiles/silo.exe.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/noxy/cpp/silo_logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/silo.exe.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/silo.exe.dir/main.cpp.o -c /home/noxy/cpp/silo_logging/main.cpp

CMakeFiles/silo.exe.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/silo.exe.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/noxy/cpp/silo_logging/main.cpp > CMakeFiles/silo.exe.dir/main.cpp.i

CMakeFiles/silo.exe.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/silo.exe.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/noxy/cpp/silo_logging/main.cpp -o CMakeFiles/silo.exe.dir/main.cpp.s

CMakeFiles/silo.exe.dir/notifier.cpp.o: CMakeFiles/silo.exe.dir/flags.make
CMakeFiles/silo.exe.dir/notifier.cpp.o: ../notifier.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/noxy/cpp/silo_logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/silo.exe.dir/notifier.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/silo.exe.dir/notifier.cpp.o -c /home/noxy/cpp/silo_logging/notifier.cpp

CMakeFiles/silo.exe.dir/notifier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/silo.exe.dir/notifier.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/noxy/cpp/silo_logging/notifier.cpp > CMakeFiles/silo.exe.dir/notifier.cpp.i

CMakeFiles/silo.exe.dir/notifier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/silo.exe.dir/notifier.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/noxy/cpp/silo_logging/notifier.cpp -o CMakeFiles/silo.exe.dir/notifier.cpp.s

CMakeFiles/silo.exe.dir/silo_result.cpp.o: CMakeFiles/silo.exe.dir/flags.make
CMakeFiles/silo.exe.dir/silo_result.cpp.o: ../silo_result.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/noxy/cpp/silo_logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/silo.exe.dir/silo_result.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/silo.exe.dir/silo_result.cpp.o -c /home/noxy/cpp/silo_logging/silo_result.cpp

CMakeFiles/silo.exe.dir/silo_result.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/silo.exe.dir/silo_result.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/noxy/cpp/silo_logging/silo_result.cpp > CMakeFiles/silo.exe.dir/silo_result.cpp.i

CMakeFiles/silo.exe.dir/silo_result.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/silo.exe.dir/silo_result.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/noxy/cpp/silo_logging/silo_result.cpp -o CMakeFiles/silo.exe.dir/silo_result.cpp.s

CMakeFiles/silo.exe.dir/transaction.cpp.o: CMakeFiles/silo.exe.dir/flags.make
CMakeFiles/silo.exe.dir/transaction.cpp.o: ../transaction.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/noxy/cpp/silo_logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/silo.exe.dir/transaction.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/silo.exe.dir/transaction.cpp.o -c /home/noxy/cpp/silo_logging/transaction.cpp

CMakeFiles/silo.exe.dir/transaction.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/silo.exe.dir/transaction.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/noxy/cpp/silo_logging/transaction.cpp > CMakeFiles/silo.exe.dir/transaction.cpp.i

CMakeFiles/silo.exe.dir/transaction.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/silo.exe.dir/transaction.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/noxy/cpp/silo_logging/transaction.cpp -o CMakeFiles/silo.exe.dir/transaction.cpp.s

CMakeFiles/silo.exe.dir/util.cpp.o: CMakeFiles/silo.exe.dir/flags.make
CMakeFiles/silo.exe.dir/util.cpp.o: ../util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/noxy/cpp/silo_logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/silo.exe.dir/util.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/silo.exe.dir/util.cpp.o -c /home/noxy/cpp/silo_logging/util.cpp

CMakeFiles/silo.exe.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/silo.exe.dir/util.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/noxy/cpp/silo_logging/util.cpp > CMakeFiles/silo.exe.dir/util.cpp.i

CMakeFiles/silo.exe.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/silo.exe.dir/util.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/noxy/cpp/silo_logging/util.cpp -o CMakeFiles/silo.exe.dir/util.cpp.s

# Object files for target silo.exe
silo_exe_OBJECTS = \
"CMakeFiles/silo.exe.dir/log_buffer.cpp.o" \
"CMakeFiles/silo.exe.dir/logger.cpp.o" \
"CMakeFiles/silo.exe.dir/main.cpp.o" \
"CMakeFiles/silo.exe.dir/notifier.cpp.o" \
"CMakeFiles/silo.exe.dir/silo_result.cpp.o" \
"CMakeFiles/silo.exe.dir/transaction.cpp.o" \
"CMakeFiles/silo.exe.dir/util.cpp.o"

# External object files for target silo.exe
silo_exe_EXTERNAL_OBJECTS =

silo.exe: CMakeFiles/silo.exe.dir/log_buffer.cpp.o
silo.exe: CMakeFiles/silo.exe.dir/logger.cpp.o
silo.exe: CMakeFiles/silo.exe.dir/main.cpp.o
silo.exe: CMakeFiles/silo.exe.dir/notifier.cpp.o
silo.exe: CMakeFiles/silo.exe.dir/silo_result.cpp.o
silo.exe: CMakeFiles/silo.exe.dir/transaction.cpp.o
silo.exe: CMakeFiles/silo.exe.dir/util.cpp.o
silo.exe: CMakeFiles/silo.exe.dir/build.make
silo.exe: CMakeFiles/silo.exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/noxy/cpp/silo_logging/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable silo.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/silo.exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/silo.exe.dir/build: silo.exe

.PHONY : CMakeFiles/silo.exe.dir/build

CMakeFiles/silo.exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/silo.exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/silo.exe.dir/clean

CMakeFiles/silo.exe.dir/depend:
	cd /home/noxy/cpp/silo_logging/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/noxy/cpp/silo_logging /home/noxy/cpp/silo_logging /home/noxy/cpp/silo_logging/build /home/noxy/cpp/silo_logging/build /home/noxy/cpp/silo_logging/build/CMakeFiles/silo.exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/silo.exe.dir/depend

