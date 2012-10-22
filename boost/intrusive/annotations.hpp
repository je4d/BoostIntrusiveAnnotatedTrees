/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Jeff Snyder    2011-2012
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_ANNOTATIONS_HPP
#define BOOST_INTRUSIVE_ANNOTATIONS_HPP

#if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
#include <boost/intrusive/detail/annotations_compat.hpp>
#endif

namespace boost {
namespace intrusive {

#if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template <BOOST_INTRUSIVE_ANNOTATION_TATND>
struct annotations;
#else
template <class ...Annotations>
struct annotations;
#endif

}  //namespace intrusive
}  //namespace boost

#endif   //#ifndef BOOST_INTRUSIVE_ANNOTATIONS_HPP
