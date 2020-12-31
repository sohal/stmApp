
project(pilotBoot ASM C CXX)

set(MCU_PART "STM32F031x6")

# USART bootloader sources
set( USART_SRC
	${SRC_PATH}/Usart1.c
)

# target lib
add_library(usart1 STATIC
	${USART_SRC}
	${BOOT_COMMON_SOURCES}
)

target_include_directories(usart1 PUBLIC
	${INC_PATH}
)

target_compile_definitions(usart1 PRIVATE
	-DSELECT_PILOT 
	-D${MCU_PART}
)

# target executable
add_executable(${PROJECT_NAME}	
	${SRC_PATH}/main.c
	${FRAMEWORK_SRC}
)

target_link_libraries(${PROJECT_NAME}
	usart1
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	-DSELECT_PILOT 
	-D${MCU_PART}
)

firmware_size(${PROJECT_NAME})
generate_object(${PROJECT_NAME} .hex ihex)
generate_object(${PROJECT_NAME} .bin binary)
generate_lst(${PROJECT_NAME})

include(cmake/flasher.cmake)

