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

#ifndef BOOST_INTRUSIVE_MONOID_ANNOTATION_HPP
#define BOOST_INTRUSIVE_MONOID_ANNOTATION_HPP
#include <boost/intrusive/semigroup_annotation.hpp>

namespace boost {
namespace intrusive {
template <class Tag, class T, class Op, T Identity, T InputValue>
struct fixed_value_monoid_annotation :
   fixed_value_semigroup_annotation<Tag, T, Op, InputValue>
{
   static const bool has_identity = true;
   static const T    identity     = Identity;
};

// Disabled as stored values are not supported yet
/*
template <class Tag, class T, class Op, T Identity>
struct stored_value_monoid_annotation :
   stored_value_semigroup_annotation<Tag, T, Op>
{
   static const bool has_identity = true;
   static const T    identity     = Identity;
};
*/

/* If the input value for an annotation comes from a function of the form T value(const ValueType&),
 * (where ValueType is the tree's value type) then use this */
template <class Tag, class T, class Op, T Identity, class InputFunctor>
struct function_monoid_annotation :
   function_semigroup_annotation<Tag, T, Op, InputFunctor>
{
   static const bool has_identity = true;
   static const T    identity     = Identity;
};

} // namespace intrusive
} // namespace boost

#endif // MONOID_ANNOTATION_HPP
