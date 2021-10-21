#------------------------------------------------------------------------------#
#  @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
# /@@/////  /@@          @@////@@ @@////// /@@
# /@@       /@@  @@@@@  @@    // /@@       /@@
# /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
# /@@////   /@@/@@@@@@@/@@       ////////@@/@@
# /@@       /@@/@@//// //@@    @@       /@@/@@
# /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
# //       ///  //////   //////  ////////  //
#
# Copyright (c) 2016, Triad National Security, LLC
# All rights reserved
#------------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.12...3.19)

#------------------------------------------------------------------------------#
# Add local module path.
#------------------------------------------------------------------------------#

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#------------------------------------------------------------------------------#
# Project.
#------------------------------------------------------------------------------#

project(FleCSI-Spack)

#------------------------------------------------------------------------------#
# Allow user to set remote fork
#------------------------------------------------------------------------------#

set(USER $ENV{USER})
message(STATUS "User: ${USER}")
set(GIT_REMOTE "https://github.com/${USER}/spack.git" CACHE STRING
  "Set the push remote.")

#------------------------------------------------------------------------------#
# Create deploy target.
#------------------------------------------------------------------------------#

find_package(Git)

if(NOT GIT_FOUND)
  message(FATAL_ERROR "Git is required for this target")
endif()

execute_process(
  COMMAND
    ${GIT_EXECUTABLE} describe
  OUTPUT_VARIABLE
    _TAG
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

include(subfilelist)
make_subfilelist(package_list ${CMAKE_SOURCE_DIR}/spack-repo/packages/*)

add_custom_target(deploy
  COMMAND
    [ -e spack ] ||
    ${GIT_EXECUTABLE} clone --single-branch --branch develop
      https://github.com/spack/spack.git &&
    cd spack &&
    ${GIT_EXECUTABLE} remote set-url --push origin ${GIT_REMOTE}
)

foreach(_PACKAGE ${package_list})
  get_filename_component(_DIRECTORY ${_PACKAGE} DIRECTORY)
  string(REGEX REPLACE "\.\.\/" "" _PACKAGE_NAME ${_DIRECTORY})
  add_custom_command(TARGET deploy POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/spack-repo/packages/${_PACKAGE_NAME}/package.py"
        "spack/var/spack/repos/builtin/packages/${_PACKAGE_NAME}/"
  )
endforeach()

add_custom_command(TARGET deploy POST_BUILD
  COMMAND
      ${GIT_EXECUTABLE} fetch &&
      ${GIT_EXECUTABLE} checkout -b ${_TAG} origin/develop &&
      ${GIT_EXECUTABLE} add -u &&
      ${GIT_EXECUTABLE} commit -m "Updating FleCSI Packages"
      ${GIT_EXECUTABLE} push
  WORKING_DIRECTORY
    spack
)
