/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Jeff Snyder    2012
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_DETAIL_BASIC_ANNOTATED_NODE_HPP
#define BOOST_INTRUSIVE_DETAIL_BASIC_ANNOTATED_NODE_HPP

#include <boost/pointer_to_other.hpp>
#include <boost/intrusive/detail/generic_annotated_node.hpp>
#include <boost/intrusive/detail/generic_annotation_list.hpp>

namespace boost {
namespace intrusive {
namespace detail {

template <class NodeTraits, class AnnotationList>
struct basic_annotated_node :
   public generic_annotated_node
      < NodeTraits
      , generic_annotation_list_traits
         < typename boost::pointer_to_other<typename NodeTraits::node_ptr, void>::type
         , AnnotationList
         >
      >
{};

template <class NodeTraits, class AnnotationList>
struct basic_annotated_node_traits :
   public generic_annotated_node_traits
      < NodeTraits
      , generic_annotation_list_traits
         < typename boost::pointer_to_other<typename NodeTraits::node_ptr, void>::type
         , AnnotationList
         >
      >
{};

} //namespace detail
} //namespace intrusive
} //namespace boost

#endif // BOOST_INTRUSIVE_DETAIL_BASIC_ANNOTATED_NODE_HPP
