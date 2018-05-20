# Important to specify system name
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Not so important to specify version number
set(CMAKE_SYSTEM_VERSION 1)

# Set the linker flags in the build
# without this flag set, CMake is not able to pass test compilation flags
set(CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nosys.specs")

# Specify toolchain prefix
set(TC_PREFIX "arm-none-eabi-")

# Specify location of toolchain root folder
# set(TC_ROOT_FOLDER "/home/p/opt/gcc4cortex")
set(TC_ROOT_FOLDER "/usr")
# Specify cross compiler 
set(CMAKE_C_COMPILER    ${TC_ROOT_FOLDER}/bin/${TC_PREFIX}gcc)
set(CMAKE_CXX_COMPILER  ${TC_ROOT_FOLDER}/bin/${TC_PREFIX}g++)
set(DEBUGGER 		${TC_ROOT_FOLDER}/bin/${TC_PREFIX}gdb)
# Compiler common flags
set (CMAKE_C_FLAGS  " -g -O2 -Wall ")
set (CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m ")
set (CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -ffreestanding -nostdlib ")
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

# Linker common flags


set(CMAKE_OBJCOPY ${TC_ROOT_FOLDER}/bin/${TC_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${TC_ROOT_FOLDER}/bin/${TC_PREFIX}size CACHE INTERNAL "size tool")

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH "${TC_ROOT_FOLDER}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
