# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/dlr/Downloads/High_Performance_Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dlr/Downloads/High_Performance_Server/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/client.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/client.cpp.o: ../client.cpp
CMakeFiles/client.dir/client.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/client.cpp.o -MF CMakeFiles/client.dir/client.cpp.o.d -o CMakeFiles/client.dir/client.cpp.o -c /home/dlr/Downloads/High_Performance_Server/client.cpp

CMakeFiles/client.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/client.cpp > CMakeFiles/client.dir/client.cpp.i

CMakeFiles/client.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/client.cpp -o CMakeFiles/client.dir/client.cpp.s

CMakeFiles/client.dir/src/Config.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Config.cpp.o: ../src/Config.cpp
CMakeFiles/client.dir/src/Config.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client.dir/src/Config.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Config.cpp.o -MF CMakeFiles/client.dir/src/Config.cpp.o.d -o CMakeFiles/client.dir/src/Config.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Config.cpp

CMakeFiles/client.dir/src/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Config.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Config.cpp > CMakeFiles/client.dir/src/Config.cpp.i

CMakeFiles/client.dir/src/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Config.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Config.cpp -o CMakeFiles/client.dir/src/Config.cpp.s

CMakeFiles/client.dir/src/ThreadPool.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/ThreadPool.cpp.o: ../src/ThreadPool.cpp
CMakeFiles/client.dir/src/ThreadPool.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client.dir/src/ThreadPool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/ThreadPool.cpp.o -MF CMakeFiles/client.dir/src/ThreadPool.cpp.o.d -o CMakeFiles/client.dir/src/ThreadPool.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/ThreadPool.cpp

CMakeFiles/client.dir/src/ThreadPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/ThreadPool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/ThreadPool.cpp > CMakeFiles/client.dir/src/ThreadPool.cpp.i

CMakeFiles/client.dir/src/ThreadPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/ThreadPool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/ThreadPool.cpp -o CMakeFiles/client.dir/src/ThreadPool.cpp.s

CMakeFiles/client.dir/src/MysqlManager.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/MysqlManager.cpp.o: ../src/MysqlManager.cpp
CMakeFiles/client.dir/src/MysqlManager.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/client.dir/src/MysqlManager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/MysqlManager.cpp.o -MF CMakeFiles/client.dir/src/MysqlManager.cpp.o.d -o CMakeFiles/client.dir/src/MysqlManager.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/MysqlManager.cpp

CMakeFiles/client.dir/src/MysqlManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/MysqlManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/MysqlManager.cpp > CMakeFiles/client.dir/src/MysqlManager.cpp.i

CMakeFiles/client.dir/src/MysqlManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/MysqlManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/MysqlManager.cpp -o CMakeFiles/client.dir/src/MysqlManager.cpp.s

CMakeFiles/client.dir/src/Manager.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Manager.cpp.o: ../src/Manager.cpp
CMakeFiles/client.dir/src/Manager.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/client.dir/src/Manager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Manager.cpp.o -MF CMakeFiles/client.dir/src/Manager.cpp.o.d -o CMakeFiles/client.dir/src/Manager.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Manager.cpp

CMakeFiles/client.dir/src/Manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Manager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Manager.cpp > CMakeFiles/client.dir/src/Manager.cpp.i

CMakeFiles/client.dir/src/Manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Manager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Manager.cpp -o CMakeFiles/client.dir/src/Manager.cpp.s

CMakeFiles/client.dir/src/Server.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Server.cpp.o: ../src/Server.cpp
CMakeFiles/client.dir/src/Server.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/client.dir/src/Server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Server.cpp.o -MF CMakeFiles/client.dir/src/Server.cpp.o.d -o CMakeFiles/client.dir/src/Server.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Server.cpp

CMakeFiles/client.dir/src/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Server.cpp > CMakeFiles/client.dir/src/Server.cpp.i

CMakeFiles/client.dir/src/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Server.cpp -o CMakeFiles/client.dir/src/Server.cpp.s

CMakeFiles/client.dir/src/Channel.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Channel.cpp.o: ../src/Channel.cpp
CMakeFiles/client.dir/src/Channel.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/client.dir/src/Channel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Channel.cpp.o -MF CMakeFiles/client.dir/src/Channel.cpp.o.d -o CMakeFiles/client.dir/src/Channel.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Channel.cpp

CMakeFiles/client.dir/src/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Channel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Channel.cpp > CMakeFiles/client.dir/src/Channel.cpp.i

CMakeFiles/client.dir/src/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Channel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Channel.cpp -o CMakeFiles/client.dir/src/Channel.cpp.s

CMakeFiles/client.dir/src/Buffer.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Buffer.cpp.o: ../src/Buffer.cpp
CMakeFiles/client.dir/src/Buffer.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/client.dir/src/Buffer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Buffer.cpp.o -MF CMakeFiles/client.dir/src/Buffer.cpp.o.d -o CMakeFiles/client.dir/src/Buffer.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Buffer.cpp

CMakeFiles/client.dir/src/Buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Buffer.cpp > CMakeFiles/client.dir/src/Buffer.cpp.i

CMakeFiles/client.dir/src/Buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Buffer.cpp -o CMakeFiles/client.dir/src/Buffer.cpp.s

CMakeFiles/client.dir/src/Epoll.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Epoll.cpp.o: ../src/Epoll.cpp
CMakeFiles/client.dir/src/Epoll.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/client.dir/src/Epoll.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Epoll.cpp.o -MF CMakeFiles/client.dir/src/Epoll.cpp.o.d -o CMakeFiles/client.dir/src/Epoll.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Epoll.cpp

CMakeFiles/client.dir/src/Epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Epoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Epoll.cpp > CMakeFiles/client.dir/src/Epoll.cpp.i

CMakeFiles/client.dir/src/Epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Epoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Epoll.cpp -o CMakeFiles/client.dir/src/Epoll.cpp.s

CMakeFiles/client.dir/src/InetAddress.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/InetAddress.cpp.o: ../src/InetAddress.cpp
CMakeFiles/client.dir/src/InetAddress.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/client.dir/src/InetAddress.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/InetAddress.cpp.o -MF CMakeFiles/client.dir/src/InetAddress.cpp.o.d -o CMakeFiles/client.dir/src/InetAddress.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/InetAddress.cpp

CMakeFiles/client.dir/src/InetAddress.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/InetAddress.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/InetAddress.cpp > CMakeFiles/client.dir/src/InetAddress.cpp.i

CMakeFiles/client.dir/src/InetAddress.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/InetAddress.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/InetAddress.cpp -o CMakeFiles/client.dir/src/InetAddress.cpp.s

CMakeFiles/client.dir/src/Socket.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Socket.cpp.o: ../src/Socket.cpp
CMakeFiles/client.dir/src/Socket.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/client.dir/src/Socket.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Socket.cpp.o -MF CMakeFiles/client.dir/src/Socket.cpp.o.d -o CMakeFiles/client.dir/src/Socket.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Socket.cpp

CMakeFiles/client.dir/src/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Socket.cpp > CMakeFiles/client.dir/src/Socket.cpp.i

CMakeFiles/client.dir/src/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Socket.cpp -o CMakeFiles/client.dir/src/Socket.cpp.s

CMakeFiles/client.dir/src/Util.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Util.cpp.o: ../src/Util.cpp
CMakeFiles/client.dir/src/Util.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/client.dir/src/Util.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Util.cpp.o -MF CMakeFiles/client.dir/src/Util.cpp.o.d -o CMakeFiles/client.dir/src/Util.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Util.cpp

CMakeFiles/client.dir/src/Util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Util.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Util.cpp > CMakeFiles/client.dir/src/Util.cpp.i

CMakeFiles/client.dir/src/Util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Util.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Util.cpp -o CMakeFiles/client.dir/src/Util.cpp.s

CMakeFiles/client.dir/src/EventLoop.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/EventLoop.cpp.o: ../src/EventLoop.cpp
CMakeFiles/client.dir/src/EventLoop.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/client.dir/src/EventLoop.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/EventLoop.cpp.o -MF CMakeFiles/client.dir/src/EventLoop.cpp.o.d -o CMakeFiles/client.dir/src/EventLoop.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/EventLoop.cpp

CMakeFiles/client.dir/src/EventLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/EventLoop.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/EventLoop.cpp > CMakeFiles/client.dir/src/EventLoop.cpp.i

CMakeFiles/client.dir/src/EventLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/EventLoop.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/EventLoop.cpp -o CMakeFiles/client.dir/src/EventLoop.cpp.s

CMakeFiles/client.dir/src/Acceptor.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Acceptor.cpp.o: ../src/Acceptor.cpp
CMakeFiles/client.dir/src/Acceptor.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/client.dir/src/Acceptor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Acceptor.cpp.o -MF CMakeFiles/client.dir/src/Acceptor.cpp.o.d -o CMakeFiles/client.dir/src/Acceptor.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Acceptor.cpp

CMakeFiles/client.dir/src/Acceptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Acceptor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Acceptor.cpp > CMakeFiles/client.dir/src/Acceptor.cpp.i

CMakeFiles/client.dir/src/Acceptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Acceptor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Acceptor.cpp -o CMakeFiles/client.dir/src/Acceptor.cpp.s

CMakeFiles/client.dir/src/Connection.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/Connection.cpp.o: ../src/Connection.cpp
CMakeFiles/client.dir/src/Connection.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/client.dir/src/Connection.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/Connection.cpp.o -MF CMakeFiles/client.dir/src/Connection.cpp.o.d -o CMakeFiles/client.dir/src/Connection.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/Connection.cpp

CMakeFiles/client.dir/src/Connection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/Connection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/Connection.cpp > CMakeFiles/client.dir/src/Connection.cpp.i

CMakeFiles/client.dir/src/Connection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/Connection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/Connection.cpp -o CMakeFiles/client.dir/src/Connection.cpp.s

CMakeFiles/client.dir/src/BufferPool.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/BufferPool.cpp.o: ../src/BufferPool.cpp
CMakeFiles/client.dir/src/BufferPool.cpp.o: CMakeFiles/client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/client.dir/src/BufferPool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client.dir/src/BufferPool.cpp.o -MF CMakeFiles/client.dir/src/BufferPool.cpp.o.d -o CMakeFiles/client.dir/src/BufferPool.cpp.o -c /home/dlr/Downloads/High_Performance_Server/src/BufferPool.cpp

CMakeFiles/client.dir/src/BufferPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/src/BufferPool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dlr/Downloads/High_Performance_Server/src/BufferPool.cpp > CMakeFiles/client.dir/src/BufferPool.cpp.i

CMakeFiles/client.dir/src/BufferPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/src/BufferPool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dlr/Downloads/High_Performance_Server/src/BufferPool.cpp -o CMakeFiles/client.dir/src/BufferPool.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/client.cpp.o" \
"CMakeFiles/client.dir/src/Config.cpp.o" \
"CMakeFiles/client.dir/src/ThreadPool.cpp.o" \
"CMakeFiles/client.dir/src/MysqlManager.cpp.o" \
"CMakeFiles/client.dir/src/Manager.cpp.o" \
"CMakeFiles/client.dir/src/Server.cpp.o" \
"CMakeFiles/client.dir/src/Channel.cpp.o" \
"CMakeFiles/client.dir/src/Buffer.cpp.o" \
"CMakeFiles/client.dir/src/Epoll.cpp.o" \
"CMakeFiles/client.dir/src/InetAddress.cpp.o" \
"CMakeFiles/client.dir/src/Socket.cpp.o" \
"CMakeFiles/client.dir/src/Util.cpp.o" \
"CMakeFiles/client.dir/src/EventLoop.cpp.o" \
"CMakeFiles/client.dir/src/Acceptor.cpp.o" \
"CMakeFiles/client.dir/src/Connection.cpp.o" \
"CMakeFiles/client.dir/src/BufferPool.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/client.cpp.o
client: CMakeFiles/client.dir/src/Config.cpp.o
client: CMakeFiles/client.dir/src/ThreadPool.cpp.o
client: CMakeFiles/client.dir/src/MysqlManager.cpp.o
client: CMakeFiles/client.dir/src/Manager.cpp.o
client: CMakeFiles/client.dir/src/Server.cpp.o
client: CMakeFiles/client.dir/src/Channel.cpp.o
client: CMakeFiles/client.dir/src/Buffer.cpp.o
client: CMakeFiles/client.dir/src/Epoll.cpp.o
client: CMakeFiles/client.dir/src/InetAddress.cpp.o
client: CMakeFiles/client.dir/src/Socket.cpp.o
client: CMakeFiles/client.dir/src/Util.cpp.o
client: CMakeFiles/client.dir/src/EventLoop.cpp.o
client: CMakeFiles/client.dir/src/Acceptor.cpp.o
client: CMakeFiles/client.dir/src/Connection.cpp.o
client: CMakeFiles/client.dir/src/BufferPool.cpp.o
client: CMakeFiles/client.dir/build.make
client: /usr/lib/libmysqlclient.so
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dlr/Downloads/High_Performance_Server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Linking CXX executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client
.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/dlr/Downloads/High_Performance_Server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dlr/Downloads/High_Performance_Server /home/dlr/Downloads/High_Performance_Server /home/dlr/Downloads/High_Performance_Server/build /home/dlr/Downloads/High_Performance_Server/build /home/dlr/Downloads/High_Performance_Server/build/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

