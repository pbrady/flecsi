#
#   @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
#  /@@/////  /@@          @@////@@ @@////// /@@
#  /@@       /@@  @@@@@  @@    // /@@       /@@
#  /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
#  /@@////   /@@/@@@@@@@/@@       ////////@@/@@
#  /@@       /@@/@@//// //@@    @@       /@@/@@
#  /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
#  //       ///  //////   //////  ////////  //
#
#  Copyright (c) 2016, Los Alamos National Security, LLC
#  All rights reserved.
#

include(CMakeDependentOption)

option(ENABLE_FLOG "Enable FleCSI Logging Utility (FLOG)" OFF)

cmake_dependent_option(FLOG_ENABLE_COLOR_OUTPUT
  "Enable colorized flog logging" ON "ENABLE_FLOG" OFF)
cmake_dependent_option(FLOG_ENABLE_TAGS "Enable tag groups" ON
  "ENABLE_FLOG;ENABLE_BOOST" OFF)
cmake_dependent_option(FLOG_ENABLE_MPI "Enable flog MPI support" ON
  "ENABLE_FLOG;ENABLE_MPI" OFF)
cmake_dependent_option(FLOG_ENABLE_DEBUG "Enable flog debug mode" OFF
  "ENABLE_FLOG" OFF)
cmake_dependent_option(FLOG_ENABLE_EXTERNAL
  "Enable messages that are defined at file scope" OFF "ENABLE_FLOG" OFF)
set(FLOG_TAG_BITS "64" CACHE STRING
  "Select the number of bits to use for tag groups")

mark_as_advanced(FLOG_ENABLE_DEBUG)
mark_as_advanced(FLOG_ENABLE_EXTERNAL)
mark_as_advanced(FLOG_TAG_BITS)

if(ENABLE_FLOG)
  set(FLOG_STRIP_LEVEL "0" CACHE STRING "Set the clog strip level")
endif()

if(FLOG_ENABLE_MPI)
  find_package(Threads)
  list(APPEND CINCH_RUNTIME_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
endif()
