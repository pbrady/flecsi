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

cmake_minimum_required(VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(engine)
include(image)

project(FleCSI-CI LANGUAGES NONE)

#------------------------------------------------------------------------------#
# Options
#------------------------------------------------------------------------------#

set(IMAGE_REPOSITORY "https://gitlab.lanl.gov/flecsi/flecsi.git" CACHE STRING
  "Set the FleCSI image repository")
set(IMAGE_BRANCH "images" CACHE STRING "Set the FleCSI image repository branch")

set(SPACK_REPOSITORY "https://github.com/spack/spack.git" CACHE STRING
  "Set the Spack repository")
set(SPACK_BRANCH "develop" CACHE STRING "Set the Spack branch")

set(SOURCE_REPOSITORY "https://gitlab.lanl.gov/flecsi/flecsi.git" CACHE STRING
  "Set the FleCSI source repository")
set(SOURCE_BRANCH "devel" CACHE STRING
  "Set the source repository branch")

set(REGISTRY "gitlab.lanl.gov:5050/flecsi/flecsi" CACHE STRING
  "Set the registry")

#------------------------------------------------------------------------------#
# Images
#------------------------------------------------------------------------------#

list(APPEND BASE_IMAGES
  centos-8
  fedora-30
  fedora-32
  fedora-33
  ubuntu-20.04
  clearlinux
)

foreach(image ${BASE_IMAGES})

  add_image(${image} ${REGISTRY}:${image} os/${image}
    ARGS
      IMAGE_REPO=${IMAGE_REPOSITORY}
      IMAGE_BRANCH=${IMAGE_BRANCH}
      SPACK_REPO=${SPACK_REPOSITORY}
      SPACK_BRANCH=${SPACK_BRANCH}
    )

  add_image(devel-${image} ${REGISTRY}:devel-${image} devel/ci
    ARGS
      SOURCE_REPO=${SOURCE_REPOSITORY}
      SOURCE_BRANCH=${SOURCE_BRANCH}
      SUBDIR=devel
      OS=${REGISTRY}:${image}
    DEPENDS push-${image}
  )

  add_image(util-${image} ${REGISTRY}:util-${image} util/ci
    ARGS
      IMAGE_REPO=${IMAGE_REPOSITORY}
      IMAGE_BRANCH=${IMAGE_BRANCH}
      SPACK_REPO=${SPACK_REPOSITORY}
      SPACK_BRANCH=${SPACK_BRANCH}
      SUBDIR=util
      OS=${REGISTRY}:${image}
    DEPENDS push-${image}
  )

  add_image(flecsi-${image} ${REGISTRY}:flecsi-${image} devel/flecsi
    ARGS
      OS=${REGISTRY}:devel-${image}
    DEPENDS push-devel-${image}
  )

endforeach()

#------------------------------------------------------------------------------#
# Tutorial image
#------------------------------------------------------------------------------#

add_image(tutorial ${REGISTRY}:tutorial
  devel/tutorial
  ARGS
  OS=${REGISTRY}:devel-clearlinux
  DEPENDS push-devel-clearlinux
)

#------------------------------------------------------------------------------#
# Legion unit test image
#------------------------------------------------------------------------------#

add_image(legion-unit ${REGISTRY}:legion-unit
  devel/legion-unit
  ARGS
    OS=${REGISTRY}:devel-ubuntu-20.04
  DEPENDS push-devel-ubuntu-20.04
)
