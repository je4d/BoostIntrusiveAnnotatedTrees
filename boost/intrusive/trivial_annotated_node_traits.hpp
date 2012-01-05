/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2006-2009
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_TRIVIAL_ANNOTATED_NODE_TRAITS_HPP
#define BOOST_INTRUSIVE_TRIVIAL_ANNOTATED_NODE_TRAITS_HPP

#include <boost/intrusive/link_mode.hpp>

namespace boost {
namespace intrusive {

//!This monid traits template is used to create monid traits
//!from user defined node traits where no annotations are used
template<class NodeTraits>
struct trivial_annotated_node_traits
{
   typedef NodeTraits                           node_traits;
   typedef typename NodeTraits::node_ptr        node_ptr;
   typedef typename NodeTraits::const_node_ptr  const_node_ptr;
   typedef typename NodeTraits::node            annotated_node;
   typedef typename NodeTraits::node_ptr        annotated_node_ptr;
   typedef typename NodeTraits::const_node_ptr  const_annotated_node_ptr;


   static node_ptr               to_node_ptr                (annotated_node_ptr mn)       { return mn; }
   static const_node_ptr         to_node_ptr                (const_annotated_node_ptr mn) { return mn; }
   static annotated_node_ptr        to_annotated_node_ptr         (node_ptr n)               { return n; }
   static const_annotated_node_ptr  to_const_annotated_node_ptr   (const_node_ptr n)         { return n; }

   template <class Annotation> static typename Annotation::type get_node_value(const_node_ptr n)
   { static_assert(std::is_class<Annotation>::value && false, "get_monoid_value called for unsupported monoid"); }
   template <class Annotation> static typename Annotation::type get_monoid_value(const_node_ptr n)
   { static_assert(std::is_class<Annotation>::value && false, "get_monoid_value called for unsupported monoid"); }
   template <class Annotation> static void set_monoid_value(node_ptr n, typename Annotation::type value)
   { static_assert(std::is_class<Annotation>::value && false, "set_monoid_value called for unsupported monoid"); }
};

} //namespace intrusive 
} //namespace boost 

#endif //BOOST_INTRUSIVE_TRIVIAL_ANNOTATED_NODE_TRAITS_HPP
