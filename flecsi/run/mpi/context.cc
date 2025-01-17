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

#include "flecsi/run/mpi/context.hh"
#include "flecsi/data.hh"

using namespace boost::program_options;

namespace flecsi::data {
pointers::pointers(prefixes & p, topo::claims::core & src)
  : column(src.colors()) {
  execute<expand, flecsi::mpi>(topo::claims::field(src), p.size(), **this);
}
void
pointers::expand(topo::claims::Field::accessor<ro> c,
  std::size_t w,
  mpi::claims::Field::accessor<wo> i) {
  auto [r, sz] = *c;
  i[0] = {r, sz * w};
}
} // namespace flecsi::data

namespace flecsi::run {

//----------------------------------------------------------------------------//
// Implementation of context_t::initialize.
//----------------------------------------------------------------------------//

int
context_t::initialize(int argc, char ** argv, bool dependent) {
  using util::mpi::test;

  if(dependent) {
    int provided;
    test(MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided));

    if(provided < MPI_THREAD_MULTIPLE) {
      std::cerr << "Your implementation of MPI does not support "
                   "MPI_THREAD_MULTIPLE which is required!"
                << std::endl;
      std::abort();
    }
  } // if

  std::tie(context::process_, context::processes_) = util::mpi::info();

  auto status = context::initialize_generic(argc, argv, dependent);

  if(status != success && dependent) {
    test(MPI_Finalize());
  } // if

#if defined(FLECSI_ENABLE_KOKKOS)
  if(dependent) {
    Kokkos::initialize(argc, argv);
  }
#endif

  return status;
} // initialize

//----------------------------------------------------------------------------//
// Implementation of context_t::finalize.
//----------------------------------------------------------------------------//

void
context_t::finalize() {

  context::finalize_generic();

#ifndef GASNET_CONDUIT_MPI
  if(context::initialize_dependent_) {
    util::mpi::test(MPI_Finalize());
  } // if
#endif

#if defined(FLECSI_ENABLE_KOKKOS)
  if(context::initialize_dependent_) {
    Kokkos::finalize();
  }
#endif
} // finalize

//----------------------------------------------------------------------------//
// Implementation of context_t::start.
//----------------------------------------------------------------------------//

int
context_t::start(const std::function<int()> & action) {
  context::start();

  context::threads_per_process_ = 1;
  context::threads_ = context::processes_;

  std::vector<char *> largv;
  largv.push_back(argv_[0]);

  for(auto opt = unrecognized_options_.begin();
      opt != unrecognized_options_.end();
      ++opt) {
    largv.push_back(opt->data());
  } // for

  return detail::data_guard(), action(); // guard destroyed after action call
}

} // namespace flecsi::run
