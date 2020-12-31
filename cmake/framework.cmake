# All source files to be compiled along with startup code
set ( FRAMEWORK_SRC
    "${CMAKE_SOURCE_DIR}/Device/startup_stm32f0xx.s"
    "${CMAKE_SOURCE_DIR}/1.5.0/Device/Source/system_stm32f0xx.c"
)

# The header includes
include_directories( 
    "${CMAKE_SOURCE_DIR}/1.5.0/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/1.5.0/Device/Include"
)

# Add library for stm32peripherals
set (CMAKE_EXECUTABLE_SUFFIX ".elf")

# The additional CMake flags
set (CFLAGS  " -Wall -g -std=c99 -Os ")  
set (CFLAGS "${CFLAGS} -mlittle-endian -mcpu=cortex-m0  -march=armv6-m -mthumb ")
set (CFLAGS "${CFLAGS} -ffunction-sections -fdata-sections ")
set (CFLAGS "${CFLAGS} -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map ")
set (CMAKE_C_FLAGS "${CFLAGS}")

# Linker path and flags
set (LINKERPATH     "${CMAKE_SOURCE_DIR}/Device/ldscripts"   ) 
set (CMAKE_EXE_LINKER_FLAGS " -L${LINKERPATH} -Tstm32f0.ld")

