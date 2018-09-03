# The CMake minimum version
cmake_minimum_required(VERSION 3.5)
set(CMAKE_VERBOSE_MAKEFILE OFF)
# Include utilities
include("${CMAKE_SOURCE_DIR}/utils.cmake")
# The name of the project
project( bootF072 ASM C )

# The VERA++ with CMAKE
find_package(vera++)

# Add library for stm32peripherals
set (CMAKE_EXECUTABLE_SUFFIX ".elf")

# Sources created by me
set ( BOOT_SRCS
    "${CMAKE_SOURCE_DIR}/main.c"
    "${CMAKE_SOURCE_DIR}/BSP.c"
    "${CMAKE_SOURCE_DIR}/Can.c"
    "${CMAKE_SOURCE_DIR}/CRC.c"
    "${CMAKE_SOURCE_DIR}/Flash.c"
    "${CMAKE_SOURCE_DIR}/Protocol.c"
    "${CMAKE_SOURCE_DIR}/Spi1.c"
    "${CMAKE_SOURCE_DIR}/Usart1.c"
)
set ( BOOT_INCS
    "${CMAKE_SOURCE_DIR}/BSP.h"
    "${CMAKE_SOURCE_DIR}/Can.h"
    "${CMAKE_SOURCE_DIR}/CRC.h"
    "${CMAKE_SOURCE_DIR}/Flash.h"
    "${CMAKE_SOURCE_DIR}/Protocol.h"
    "${CMAKE_SOURCE_DIR}/Spi1.h"
    "${CMAKE_SOURCE_DIR}/Usart1.h"
)
# All source files to be compiled along with startup code
set ( SOURCES 
    "${BOOT_SRCS}"
    "${CMAKE_SOURCE_DIR}/Device/startup_stm32f0xx.s"
    "${CMAKE_SOURCE_DIR}/1.5.0/Device/Source/system_stm32f0xx.c"
)

# The header includes
include_directories( 
    "${CMAKE_SOURCE_DIR}/"
    "${CMAKE_SOURCE_DIR}/1.5.0/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/1.5.0/Device/Include"
)
# The additional CMake flags
set (CFLAGS  " -Wall -g -std=c99 -Os ")  
set (CFLAGS "${CFLAGS} -mlittle-endian -mcpu=cortex-m0  -march=armv6-m -mthumb ")
set (CFLAGS "${CFLAGS} -ffunction-sections -fdata-sections ")
set (CFLAGS "${CFLAGS} -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map ")
set (CMAKE_C_FLAGS "${CFLAGS}")

# Linker path and flags
set (LINKERPATH     "${CMAKE_SOURCE_DIR}/Device/ldscripts"   ) 
set (CMAKE_EXE_LINKER_FLAGS " -L${LINKERPATH} -Tstm32f0.ld")

# Make target here
add_executable( bootPilot
    ${SOURCES}
)
add_executable( bootTorque
    ${SOURCES}
)
add_executable( bootCAN
    ${SOURCES}
)
add_executable( bootSPI
    ${SOURCES}
)

add_executable( ${PROJECT_NAME}
    ${SOURCES}
)

# target specific options
target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE -DSELECT_SPI -DSTM32F072xB
)

target_compile_definitions(
    bootPilot
    PRIVATE -DSELECT_PILOT -DSTM32F031x6
)
target_compile_definitions(
    bootTorque
    PRIVATE -DSELECT_TORQUE -DSTM32F031x6
)
target_compile_definitions(
    bootCAN
    PRIVATE -DSELECT_CAN -DSTM32F042x6
)
target_compile_definitions(
    bootSPI
    PRIVATE -DSELECT_SPI -DSTM32F031x6
)
# Print Firmware Size
firmware_size(${PROJECT_NAME})
firmware_size(bootCAN)
firmware_size(bootPilot)
firmware_size(bootTorque)
firmware_size(bootSPI)

# Generate Object additionally
generate_object(bootSPI .hex ihex)
generate_object(bootCAN .hex ihex)
generate_object(bootPilot .hex ihex)
generate_object(bootTorque .hex ihex)
generate_object(${PROJECT_NAME} .hex ihex)

# Generate Object additionally
generate_object(${PROJECT_NAME} .bin binary)
generate_object(bootCAN .bin binary)
generate_object(bootTorque .bin binary)
generate_object(bootPilot .bin binary)
generate_object(bootSPI .bin binary)

# Generate Lst
generate_lst(${PROJECT_NAME})
generate_lst(bootCAN)
generate_lst(bootPilot)
generate_lst(bootTorque)
generate_lst(bootSPI)

# Configure flasher script for the project
set(BINARY ${PROJECT_NAME}.bin)
set(FLASH_START 0x08000000)

# ST-Link flash command
add_custom_target(flash 
	COMMAND st-flash --reset write ${BINARY} ${FLASH_START}
	DEPENDS ${PROJECT_NAME}
)

# ST-Link erase command
add_custom_target(erase 
	COMMAND st-flash erase
	DEPENDS ${PROJECT_NAME}
)

# ST-Link debugger command
add_custom_target(debug 
	COMMAND ${DEBUGGER} -tui -command ${CMAKE_SOURCE_DIR}/remote.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} 
	DEPENDS ${PROJECT_NAME}
)

# ST-Link debug server command
add_custom_target(debug-server 
	COMMAND st-util --listen_port 2331
	DEPENDS ${PROJECT_NAME}
)

# Vera++ custom target
include (${VERA++_USE_FILE})
add_vera_targets(
  ${BOOT_SRCS}
  ${BOOT_INCS}
  RECURSE
  ROOT "${CMAKE_SOURCE_DIR}/.vera++/"
)
