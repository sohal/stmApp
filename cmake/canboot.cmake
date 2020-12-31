
project(CANBoot ASM C CXX)

set(MCU_PART "STM32F042x6")

# CAN bootloader sources
set( CAN_SRC
	${SRC_PATH}/Can.c
)

# target lib
add_library(can STATIC
	${CAN_SRC}
	${BOOT_COMMON_SOURCES}
)

target_include_directories(can PUBLIC
	${INC_PATH}
)

target_compile_definitions(can PUBLIC
	-DSELECT_CAN 
	-D${MCU_PART}
)

# target executable
add_executable(${PROJECT_NAME}	
	${SRC_PATH}/main.c
	${FRAMEWORK_SRC}
)

target_link_libraries(${PROJECT_NAME}
	can
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
	-DSELECT_CAN 
	-D${MCU_PART}
)

firmware_size(${PROJECT_NAME})
generate_object(${PROJECT_NAME} .hex ihex)
generate_object(${PROJECT_NAME} .bin binary)
generate_lst(${PROJECT_NAME})

include(cmake/flasher.cmake)
