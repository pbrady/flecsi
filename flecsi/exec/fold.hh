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

#include <algorithm>
#include <limits>

namespace flecsi {
namespace exec::fold {
/// \defgroup fold Reduction Operations
/// Types to use with \c reduce and \c reduceall.
/// \ingroup execution
/// \{

#ifdef DOXYGEN
/// An example reduction type that is not really implemented.
/// The data type to use is determined separately (_e.g._, a task return
/// type); user-defined reduction types that aim to support only one data type
/// need not define their members as templates.
struct reduce {
  /// Apply the reduction to two values.
  template<class T>
  static T combine(T, T);

  /// The identity element.
  template<class T>
  static const T identity;
};
#endif

/*!
  Minimum reduction type.
 */
struct min {
  template<class T>
  static T combine(T a, T b) {
    return std::min(a, b);
  }

private:
  template<class T>
  using lim = std::numeric_limits<T>;

public:
  template<class T>
  static constexpr T identity = lim<T>::has_infinity ? lim<T>::infinity()
                                                     : lim<T>::max();
}; // struct min

/*!
  Maximum reduction type.
 */
struct max {
  template<class T>
  static T combine(T a, T b) {
    return std::max(a, b);
  }

private:
  template<class T>
  using lim = std::numeric_limits<T>;

public:
  template<class T>
  static constexpr T identity = lim<T>::has_infinity ? -lim<T>::infinity()
                                                     : lim<T>::lowest();
}; // struct max

/*!
  Sum reduction type.
 */
struct sum {
  template<class T>
  static T combine(T a, T b) {
    return a + b;
  }
  template<class T>
  static constexpr T identity = T(0);
}; // struct sum

/*!
  Product reduction type.
 */
struct product {
  template<class T>
  static T combine(T a, T b) {
    return a * b;
  }
  template<class T>
  static constexpr T identity = T(1);
}; // struct product

/// \}
} // namespace exec::fold

namespace exec::detail {
template<class R, class = void>
struct identity_traits {
  // GCC rejects this if it's a reference (#97340):
  template<class T>
  static inline const T value{R::template identity<T>};
};
template<class R>
struct identity_traits<R, decltype(void(&R::identity))> {
  template<class T>
  static inline const T & value{R::identity};
};
} // namespace exec::detail
} // namespace flecsi
