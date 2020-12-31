# The CMake minimum version
cmake_minimum_required(VERSION 3.12)
set(CMAKE_VERBOSE_MAKEFILE OFF)
# The name of the project
project(bootF072 ASM C)

# Include utilities
include(cmake/utils.cmake)
# Include STM32 specific framework
include(cmake/framework.cmake)
# Include application based common sources
include(cmake/bootcommon.cmake)
# Include application targets
include(cmake/spiboot.cmake)
include(cmake/canboot.cmake)
include(cmake/torqueboot.cmake)
include(cmake/pilotboot.cmake)	

# Include vera
#include(cmake/vera++.cmake)

