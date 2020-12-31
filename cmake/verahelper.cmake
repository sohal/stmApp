cmake_minimum_required(VERSION 3.19)

project(vera-download NONE)

include(ExternalProject)

ExternalProject_Add(vera
    GIT_REPOSITORY https://bitbucket.org/verateam/vera.git	
    GIT_TAG master
    SOURCE_DIR "${CMAKE_BINARY_DIR}/vera-src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/vera-build"
    GIT_SUBMODULE
    CMAKE_ARGS "-DVERA_USE_SYSTEM_LUA=OFF"
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)
