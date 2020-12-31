
set(CMAKE_CXX_STANDARD 11)
# Download and unpack googletest at configure time
configure_file(cmake/verahelper.cmake vera-download/CMakeLists.txt)

execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/vera-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/vera-download"
)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/vera-src"
                 "${CMAKE_BINARY_DIR}/vera-build"
)

