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

#define __FLECSI_PRIVATE__
#include <flecsi/data/data.hh>
#include <flecsi/execution/execution.hh>
#include <flecsi/utils/demangle.hh>
#include <flecsi/utils/ftest.hh>

using namespace flecsi;

flecsi_add_index_field("test", "pressure", double, 2);
inline auto pressure = flecsi_index_field_instance("test", "pressure", double, 0);

template<size_t PRIVILEGES>
using accessor =
  flecsi::data::index_accessor_u<double, privilege_pack_u<PRIVILEGES>::value>;

namespace index_test {

void
assign(accessor<rw> ia) {
  flog(info) << "assign" << std::endl;
  ia = flecsi_color();
} // assign

flecsi_register_task(assign, index_test, loc, index);

int
check(accessor<ro> ia) {

  FTEST();

  flog(info) << "check" << std::endl;
  ASSERT_EQ(ia, flecsi_color());

  return FTEST_RESULT();
} // print

flecsi_register_task(check, index_test, loc, index);

} // namespace index_test

int
index_topology(int argc, char ** argv) {

  flecsi_execute_task(assign, index_test, index, pressure);
  flecsi_execute_task(check, index_test, index, pressure);

  return 0;
} // index

ftest_register_driver(index_topology);
