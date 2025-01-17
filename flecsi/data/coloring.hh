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

#include <flecsi/execution.hh>

#include <optional>

namespace flecsi {
namespace data {
/// \addtogroup data
/// \{

/// A coloring object, constructed on request.
///
/// \note Usually accessed as \c Topo::cslot.
template<class Topo>
struct coloring_slot {
  using color_type = typename Topo::coloring;

  /// Create the coloring object.
  /// \param args arguments to \c Topo::color
  /// \return the created object
  template<typename... ARGS>
  color_type & allocate(ARGS &&... args) {
    constexpr auto f = [](coloring_slot & s, ARGS &&... aa) {
      s.coloring.emplace(Topo::color(std::forward<ARGS>(aa)...));
    };
    execute<*f, flecsi::mpi>(*this, std::forward<ARGS>(args)...);
    return get();
  } // allocate

  /// Destroy the coloring object.
  void deallocate() {
    coloring.reset();
  } // deallocate

  /// Get the coloring object.
  color_type & get() {
    return *coloring;
  }
  /// Get the coloring object.
  const color_type & get() const {
    return *coloring;
  }

private:
  std::optional<color_type> coloring;
};

/// \}
} // namespace data
} // namespace flecsi
