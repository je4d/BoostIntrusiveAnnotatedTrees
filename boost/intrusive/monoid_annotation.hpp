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

template <class Monoid>
struct fixed_value_monoid_annotation
{
   typedef typename Monoid::type type;

   template <class AnnotatedNodeTraits>
   static void update_node(typename AnnotatedNodeTraits::annotation_list_ptr n) {
      typedef typename AnnotatedNodeTraits::annotation_list_traits
                                                         annotation_list_traits;
      typedef typename AnnotatedNodeTraits::node_traits  node_traits;
      typedef typename node_traits::node_ptr             node_ptr;
      typedef fixed_value_monoid_annotation<Monoid>      annotation;

      node_ptr left = node_traits::get_left(AnnotatedNodeTraits::to_node_ptr(n));
      node_ptr right = node_traits::get_right(AnnotatedNodeTraits::to_node_ptr(n));
      type n_value = Monoid::value;
      typename Monoid::type left_plus_value = left
         ?  typename Monoid::operation()(
               annotation_list_traits::template get_annotation_value<annotation>(
                  AnnotatedNodeTraits::to_annotation_list_ptr(left)),
               n_value)
         : n_value;
      annotation_list_traits::template set_annotation_value<annotation>(n, right
            ?  typename Monoid::operation()(
                  left_plus_value,
                  annotation_list_traits::template get_annotation_value<annotation>(
                     AnnotatedNodeTraits::to_annotation_list_ptr(right)))
            : left_plus_value);
   }
};

/* If the input value for an annotation comes from a function of the form annotation::type value(AnnotatedNode*),
 * (e.g. if the annotated node stores the input value) then use this */

template <class Monoid>
struct stored_value_monoid_annotation
{
   typename Monoid::type type;
   static const bool input_value_in_annotation_list = true;

   template <class AnnotatedNodeTraits>
   void update_node(typename AnnotatedNodeTraits::annotation_list_ptr n) {
      typedef typename AnnotatedNodeTraits::node_traits  node_traits;
      typedef typename node_traits::node_ptr             node_ptr;
      typedef stored_value_monoid_annotation<Monoid>     annotation_type;

      node_ptr left = node_traits::get_left(n);
      node_ptr right = node_traits::get_right(n);
      typename Monoid::type n_value = AnnotatedNodeTraits::get_annotation_input_value(AnnotatedNodeTraits::to_annotation_list_ptr(n));
      typename Monoid::type left_plus_value = left ?
          typename Monoid::operation()(AnnotatedNodeTraits::template get_annotation_value<annotation_type>(AnnotatedNodeTraits::to_annotation_list_ptr(left)), n_value) : n_value;
      AnnotatedNodeTraits::template set_annotation_value<annotation_type>(n, right ?
            typename annotation_type::operation()(left_plus_value, AnnotatedNodeTraits::template get_annotation_value<annotation_type>(AnnotatedNodeTraits::to_annotation_list_ptr(right))) : left_plus_value);
   }
};

/* If the input value for an annotation comes from a function of the form annotation::type value(const ValueType&),
 * then use this */
/*template <class Monoid>
struct value_function_monoid_annotation
{
   bool apply_value_traits = true;

   template <class ValueTraits>
   struct type {
      typename Monoid::type type;
      template <class AnnotatedNodeTraits>
      void update_node(typename AnnotatedNodeTraits::annotation_list_ptr n) {
         typedef typename AnnotatedNodeTraits::node_traits  node_traits;
         typedef typename node_traits::node_ptr             node_ptr;

         node_ptr left = node_traits::get_left(n);
         node_ptr right = node_traits::get_right(n);
         typename Annotation::type n_value = AnnotatedNodeTraits::get_annotation_input_value(AnnotatedNodeTraits::to_annotation_list_ptr(n));
         typename Annotation::type left_plus_value = left ?
             typename Annotation::operation()(AnnotatedNodeTraits::template get_annotation_value<Annotation>(AnnotatedNodeTraits::to_annotation_list_ptr(left)), n_value) : n_value;
         AnnotatedNodeTraits::template set_annotation_value<Annotation>(n, right ?
               typename Annotation::operation()(left_plus_value, AnnotatedNodeTraits::template get_annotation_value<Annotation>(AnnotatedNodeTraits::to_annotation_list_ptr(right))) : left_plus_value);
      }
   };
};*/

}
}

#endif // MONOID_ANNOTATION_HPP
