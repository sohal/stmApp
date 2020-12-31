cmake_minimum_required(VERSION 3.19)

project(vera-download NONE)

#include(ExternalProject)

#ExternalProject_Add(vera
#    GIT_REPOSITORY https://bitbucket.org/verateam/vera.git	
#    GIT_TAG master
#    GIT_SUBMODULE
#    INSTALL_COMMAND ""
#    TEST_COMMAND ""
#)


# Vera++ custom target
find_package(vera++)
#include (${VERA++_USE_FILE})
add_vera_targets(
  ${SRC_PATH}
  ${INC_PATH}
  RECURSE
  ROOT "${CMAKE_SOURCE_DIR}/.vera++/"
)
