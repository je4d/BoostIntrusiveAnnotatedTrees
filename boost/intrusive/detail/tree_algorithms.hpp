/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2007.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_TREE_ALGORITHMS_HPP
#define BOOST_INTRUSIVE_TREE_ALGORITHMS_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/detail/assert.hpp>
#include <boost/intrusive/intrusive_fwd.hpp>
#include <cstddef>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/intrusive/annotated_tree_algorithms.hpp>
#include <boost/intrusive/trivial_annotated_node_traits.hpp>

namespace boost {
namespace intrusive {
namespace detail {

template<class NodeTraits>
class tree_algorithms : public annotated_tree_algorithms<::boost::intrusive::trivial_annotated_node_traits<NodeTraits>>
{
};

}  //namespace detail {
}  //namespace intrusive
}  //namespace boost

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_TREE_ALGORITHMS_HPP
// vim: set sw=3 sts=3 ts=3 et:
