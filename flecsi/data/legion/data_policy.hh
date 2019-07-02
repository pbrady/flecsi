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

/*!
  @file

  This file is really just a conduit to capture the different
  specializations for data clients and storage classes.
 */

#include <flecsi/data/legion/storage_classes.hh>
#include <flecsi/data/legion/topologies.hh>

namespace flecsi {
namespace data {

struct legion_data_policy_t {

  /*--------------------------------------------------------------------------*
    Storage Class Interface.
   *--------------------------------------------------------------------------*/

  /*
    Capture the base storage class type. This is necessary as a place holder in
    the field interface.
   */

  template<size_t STORAGE_CLASS, typename TOPOLOGY_TYPE>
  using storage_class_u = legion::storage_class_u<STORAGE_CLASS, TOPOLOGY_TYPE>;

  /*--------------------------------------------------------------------------*
    Topology Instance Interface.
   *--------------------------------------------------------------------------*/

  template<typename TOPOLOGY_TYPE>
  using topology_instance_u = typename legion::topology_instance_u<
    typename TOPOLOGY_TYPE::type_identifier_t>;

  template<typename TOPOLOGY_TYPE>
  static void create(
    topology_reference_u<typename TOPOLOGY_TYPE::type_identifier_t> const &
      topology_reference,
    typename TOPOLOGY_TYPE::coloring_t const & coloring) {
    topology_instance_u<typename TOPOLOGY_TYPE::type_identifier_t>::create(
      topology_reference, coloring);
  } // create

  template<typename TOPOLOGY_TYPE>
  static void destroy(
    topology_reference_u<typename TOPOLOGY_TYPE::type_identifier_t> const &
      topology_reference) {
    topology_instance_u<typename TOPOLOGY_TYPE::type_identifier_t>::destroy(
      topology_reference);
  } // destroy

  /*--------------------------------------------------------------------------*
    Topology Accessor Interface.
   *--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*
    Field Accessor Interface.
   *--------------------------------------------------------------------------*/

  template<typename DATA_TYPE, size_t PRIVILEGES>
  using global_accessor_u =
    legion::global_topology::accessor_u<DATA_TYPE, PRIVILEGES>;

  template<typename DATA_TYPE, size_t PRIVILEGES>
  using index_accessor_u =
    legion::index_topology::accessor_u<DATA_TYPE, PRIVILEGES>;

}; // struct legion_data_policy_t

} // namespace data
} // namespace flecsi
