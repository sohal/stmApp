
project(SPIBoot ASM C CXX)

set(MCU_PART "STM32F031x6")

# SPI bootloader sources
set( SPI_SRC
	${SRC_PATH}/Spi1.c
)

# target lib
add_library(spi STATIC
	${SPI_SRC}
	${BOOT_COMMON_SOURCES}
)

target_include_directories(spi PUBLIC
	${INC_PATH}
)

target_compile_definitions(spi PRIVATE
	-DSELECT_SPI 
	-D${MCU_PART}
)

# target executable
add_executable(${PROJECT_NAME}	
	${SRC_PATH}/main.c
	${FRAMEWORK_SRC}
)

target_link_libraries(${PROJECT_NAME}
	spi
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	-DSELECT_SPI 
	-D${MCU_PART}
)

firmware_size(${PROJECT_NAME})
generate_object(${PROJECT_NAME} .hex ihex)
generate_object(${PROJECT_NAME} .bin binary)
generate_lst(${PROJECT_NAME})

include(cmake/flasher.cmake)

