# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /opt/cmake-3.11.2-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.11.2-Linux-x86_64/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mori3rti/Documents/Ccpp/stm32Command

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mori3rti/Documents/Ccpp/stm32Command

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/opt/cmake-3.11.2-Linux-x86_64/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/opt/cmake-3.11.2-Linux-x86_64/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(CMAKE_COMMAND) -E cmake_progress_start /home/mori3rti/Documents/Ccpp/stm32Command/CMakeFiles /home/mori3rti/Documents/Ccpp/stm32Command/App/Modul/CommandHandler/CMakeFiles/progress.marks
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(MAKE) -f CMakeFiles/Makefile2 App/Modul/CommandHandler/all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/mori3rti/Documents/Ccpp/stm32Command/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(MAKE) -f CMakeFiles/Makefile2 App/Modul/CommandHandler/clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(MAKE) -f CMakeFiles/Makefile2 App/Modul/CommandHandler/preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(MAKE) -f CMakeFiles/Makefile2 App/Modul/CommandHandler/preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

# Convenience name for target.
App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/rule:
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(MAKE) -f CMakeFiles/Makefile2 App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/rule
.PHONY : App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/rule

# Convenience name for target.
CommandHandler: App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/rule

.PHONY : CommandHandler

# fast build rule for target.
CommandHandler/fast:
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(MAKE) -f App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/build.make App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/build
.PHONY : CommandHandler/fast

CommandHandler.obj: CommandHandler.c.obj

.PHONY : CommandHandler.obj

# target to build an object file
CommandHandler.c.obj:
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(MAKE) -f App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/build.make App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/CommandHandler.c.obj
.PHONY : CommandHandler.c.obj

CommandHandler.i: CommandHandler.c.i

.PHONY : CommandHandler.i

# target to preprocess a source file
CommandHandler.c.i:
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(MAKE) -f App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/build.make App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/CommandHandler.c.i
.PHONY : CommandHandler.c.i

CommandHandler.s: CommandHandler.c.s

.PHONY : CommandHandler.s

# target to generate assembly for a file
CommandHandler.c.s:
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(MAKE) -f App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/build.make App/Modul/CommandHandler/CMakeFiles/CommandHandler.dir/CommandHandler.c.s
.PHONY : CommandHandler.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... CommandHandler"
	@echo "... CommandHandler.obj"
	@echo "... CommandHandler.i"
	@echo "... CommandHandler.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	cd /home/mori3rti/Documents/Ccpp/stm32Command && $(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

