/*
    @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
   /@@/////  /@@          @@////@@ @@////// /@@
   /@@       /@@  @@@@@  @@    // /@@       /@@
   /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
   /@@////   /@@/@@@@@@@/@@       ////////@@/@@
   /@@       /@@/@@//// //@@    @@       /@@/@@
   /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
   //       ///  //////   //////  ////////  //

   Copyright (c) 2016, Triad National Security, LLC
   All rights reserved.
                                                                              */
#pragma once

/*! @file */

#include <limits>

#include <flecsi-config.h>

/*----------------------------------------------------------------------------*
  This section works with the build system to select the correct runtime
  implemenation for the task model. If you add to the possible runtimes,
  remember to edit config/packages.cmake to include a definition using
  the same convention, e.g., -DFLECSI_RUNTIME_MODEL_new_runtime.
 *----------------------------------------------------------------------------*/

#if FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_legion

#include <legion.h>

namespace flecsi {

using field_id_t = Legion::FieldID;

} // namespace flecsi

#elif FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_mpi

#include <cstddef>

namespace flecsi {

using field_id_t = size_t;

} // namespace flecsi

#elif FLECSI_RUNTIME_MODEL == FLECSI_RUNTIME_MODEL_hpx

namespace flecsi {

#include <cstddef>

using field_id_t = size_t;

} // namespace flecsi

#endif // FLECSI_RUNTIME_MODEL

#include "flecsi/util/common.hh"

namespace flecsi {

/*!
  Unique counter for field ids.
 */
inline util::counter<field_id_t(FLECSI_GENERATED_ID_MAX)> fid_counter(0);

} // namespace flecsi
