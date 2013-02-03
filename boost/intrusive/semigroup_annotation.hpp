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

#ifndef BOOST_INTRUSIVE_SEMIGROUP_ANNOTATION_HPP
#define BOOST_INTRUSIVE_SEMIGROUP_ANNOTATION_HPP

#include <boost/intrusive/detail/mpl.hpp>

namespace boost {
namespace intrusive {
namespace detail {
   template <class AnnotationTag, class T, class Op, class AnnotatedNodeTraits>
   void semigroup_node_update(typename AnnotatedNodeTraits::annotation_list_ptr n, T n_value)
   {
      typedef typename AnnotatedNodeTraits::annotation_list_traits
                                                         annotation_list_traits;
      typedef typename AnnotatedNodeTraits::node_traits  node_traits;
      typedef typename node_traits::node_ptr             node_ptr;

      // result =
      node_ptr left = node_traits::get_left(AnnotatedNodeTraits::to_node_ptr(n));
      // +
      // n_value
      // +
      node_ptr right = node_traits::get_right(AnnotatedNodeTraits::to_node_ptr(n));
      T left_plus_value = left
         ?  Op()(
               annotation_list_traits::template get_annotation_value<AnnotationTag>(
                  AnnotatedNodeTraits::to_annotation_list_ptr(left)),
               n_value)
         : n_value;
      annotation_list_traits::template set_annotation_value<AnnotationTag>(n, right
            ?  Op()(
                  left_plus_value,
                  annotation_list_traits::template get_annotation_value<AnnotationTag>(
                     AnnotatedNodeTraits::to_annotation_list_ptr(right)))
            : left_plus_value);
   }
} // namespace detail

template <class Tag, class T, class Op, T InputValue>
struct fixed_value_semigroup_annotation
{
   typedef T type;

   template <class AnnotatedNodeTraits>
   static void update_node(typename AnnotatedNodeTraits::annotation_list_ptr n)
   {
      detail::semigroup_node_update
         < Tag
         , T
         , Op
         , AnnotatedNodeTraits
         > (n, InputValue);
   }
};

// Disabled as stored values are not supported yet (get/set_annotation_input_value are undefined)
/*
template <class Tag, class T, class Op>
struct stored_value_semigroup_annotation
{
   typedef T type;
   static const bool input_value_in_annotation_list = true;

   template <class AnnotatedNodeTraits>
   static void update_node(typename AnnotatedNodeTraits::annotation_list_ptr n)
   {
      detail::semigroup_node_update
         < Tag
         , T
         , Op
         , AnnotatedNodeTraits
         > (n, AnnotatedNodeTraits::get_annotation_input_value(n));
   }
};
*/

/* If the input value for an annotation comes from a function of the form annotation::type value(const ValueType&),
 * then use this */
template <class Tag, class T, class Op, class InputFunctor>
struct function_semigroup_annotation
{
   typedef T type;
   static const bool apply_config = true;


   template <class ValueTraits>
   struct real_annotation {
      typedef T type;
      static const bool explicit_annotation_hook = true;
      typedef Tag annotation_hook;

      template <class AnnotatedNodeTraits>
      static void update_node(typename AnnotatedNodeTraits::annotation_list_ptr n) {
         BOOST_STATIC_ASSERT((detail::is_same<typename ValueTraits::node_traits, typename AnnotatedNodeTraits::node_traits>::value));
         detail::semigroup_node_update
            < Tag
            , T
            , Op
            , AnnotatedNodeTraits
            > (n, InputFunctor()(*ValueTraits::to_value_ptr(
                        AnnotatedNodeTraits::to_node_ptr(n))));
      }
   };

   template <class Config>
   struct make_real_annotation {
      typedef real_annotation<typename Config::value_traits> type;
   };
};

} // namespace intrusive
} // namespace boost

#endif // SEMIGROUP_ANNOTATION_HPP
