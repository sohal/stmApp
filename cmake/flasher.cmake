
# Configure flasher script for the project
set(BINARY ${PROJECT_NAME}.bin)
set(FLASH_START 0x08000000)

# ST-Link flash command
add_custom_target(flash_${PROJECT_NAME} 
	COMMAND st-flash --reset write ${BINARY} ${FLASH_START}
	DEPENDS ${PROJECT_NAME}
)

# ST-Link erase command
add_custom_target(erase_${PROJECT_NAME} 
	COMMAND st-flash erase
	DEPENDS ${PROJECT_NAME}
)

# ST-Link debugger command
add_custom_target(debug_${PROJECT_NAME} 
	COMMAND ${DEBUGGER} -tui -command ${CMAKE_SOURCE_DIR}/remote.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/{PROJECT_NAME} 
	DEPENDS ${PROJECT_NAME}
)

# ST-Link debug server command
add_custom_target(debug-server_${PROJECT_NAME} 
	COMMAND st-util --listen_port 2331
	DEPENDS ${PROJECT_NAME}
)

