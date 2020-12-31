
project(torqueBoot ASM C CXX)

set(MCU_PART "STM32F031x6")

# USART bootloader sources
set( USART_SRC
	${SRC_PATH}/Usart1.c
)

# target lib
add_library(usart STATIC
	${USART_SRC}
	${BOOT_COMMON_SOURCES}
)

target_include_directories(usart PUBLIC
	${INC_PATH}
)

target_compile_definitions(usart PRIVATE
	-DSELECT_TORQUE 
	-D${MCU_PART}
)

# target executable
add_executable(${PROJECT_NAME}	
	${SRC_PATH}/main.c
)

target_link_libraries(${PROJECT_NAME}
	usart
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	-DSELECT_TORQUE 
	-D${MCU_PART}
)

firmware_size(${PROJECT_NAME})
generate_object(${PROJECT_NAME} .hex ihex)
generate_object(${PROJECT_NAME} .bin binary)
generate_lst(${PROJECT_NAME})

include(cmake/flasher.cmake)

