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

namespace boost {
namespace intrusive {
namespace detail {
   template <class Monoid, class AnnotationHook, class AnnotatedNodeTraits>
   void monoid_node_update(typename AnnotatedNodeTraits::annotation_list_ptr n, typename Monoid::type n_value)
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
      typename Monoid::type left_plus_value = left
         ?  typename Monoid::operation()(
               annotation_list_traits::template get_annotation_value<AnnotationHook>(
                  AnnotatedNodeTraits::to_annotation_list_ptr(left)),
               n_value)
         : n_value;
      annotation_list_traits::template set_annotation_value<AnnotationHook>(n, right
            ?  typename Monoid::operation()(
                  left_plus_value,
                  annotation_list_traits::template get_annotation_value<AnnotationHook>(
                     AnnotatedNodeTraits::to_annotation_list_ptr(right)))
            : left_plus_value);
   }

} // namespace detail

template <class Monoid>
struct fixed_value_monoid_annotation
{
   typedef typename Monoid::type type;

   template <class AnnotatedNodeTraits>
   static void update_node(typename AnnotatedNodeTraits::annotation_list_ptr n)
   {
      detail::monoid_node_update
         < Monoid
         , fixed_value_monoid_annotation<Monoid>
         , AnnotatedNodeTraits
         > (n, Monoid::value);
   }
};

template <class Monoid>
struct stored_value_monoid_annotation
{
   typename Monoid::type type;
   static const bool input_value_in_annotation_list = true;

   template <class AnnotatedNodeTraits>
   static void update_node(typename AnnotatedNodeTraits::annotation_list_ptr n)
   {
      detail::monoid_node_update
         < Monoid
         , stored_value_monoid_annotation<Monoid>
         , AnnotatedNodeTraits
         > (n, AnnotatedNodeTraits::get_annotation_input_value(n));
   }
};

/* If the input value for an annotation comes from a function of the form annotation::type value(const ValueType&),
 * then use this */
template <class Monoid>
struct function_monoid_annotation
{
   typedef typename Monoid::type type;
   static const bool apply_config = true;


   template <class ValueTraits>
   struct real_annotation {
      typedef typename Monoid::type type;
      static const bool explicit_annotation_hook = true;
      typedef function_monoid_annotation<Monoid> annotation_hook;

      template <class AnnotatedNodeTraits>
      static void update_node(typename AnnotatedNodeTraits::annotation_list_ptr n) {
         BOOST_STATIC_ASSERT((detail::is_same<typename ValueTraits::node_traits, typename AnnotatedNodeTraits::node_traits>::value));
         detail::monoid_node_update
            < Monoid
            , function_monoid_annotation<Monoid>
            , AnnotatedNodeTraits
            > (n, typename Monoid::get_input_value()(*ValueTraits::to_value_ptr(
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

#endif // MONOID_ANNOTATION_HPP
