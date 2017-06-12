/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_utils_tuple_walker_h
#define flecsi_utils_tuple_walker_h

//!
//! \file
//! \date Initial file creation: Jul 28, 2016
//!

#include <tuple>

namespace flecsi {
namespace utils {

  template<
    std::size_t I,
    typename T,
    typename P
  >
  struct tuple_walker_helper__{
    static
    std::size_t
    walk(
      P & p,
      T & t)
    {
      p.handle(std::get<std::tuple_size<T>::value - I>(t));
      return tuple_walker_helper__<I - 1, T, P>::walk(p, t);
    }
  };

  template<
    typename T,
    typename P
  >
  struct tuple_walker_helper__<0, T, P>{
    static
    std::size_t
    walk(
         P &, T &)
    {
      return 0;
    }
  };

  template<
    typename P
  >
  struct tuple_walker__{
    template<
      typename T
    >
    void walk(
      T & t)
    {
      tuple_walker_helper__<std::tuple_size<T>::value, T, P>::
        walk(*static_cast<P*>(this), t);
    }
  };

} // namespace utils
} // namespace flecsi

#endif // flecsi_utils_tuple_walker_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
