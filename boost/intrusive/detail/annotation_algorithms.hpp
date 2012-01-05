/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Jeff Snyder    2010-2011
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_DETAIL_ANNOTATION_ALGORITHMS_HPP
#define BOOST_INTRUSIVE_DETAIL_ANNOTATION_ALGORITHMS_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/identity.hpp>

namespace boost {
namespace intrusive {
namespace detail {

template <class T>
class tree_algorithms;

template <class AnnotatedNodeTraits, class Annotation>
struct annotation_algorithms
{
//   typedef typename AnnotatedNodeTraits::node_traits               node_traits;
//   typedef typename node_traits::node_ptr                          node_ptr;
//   typedef typename node_traits::const_node_ptr                    const_node_ptr;
   typedef typename AnnotatedNodeTraits::annotated_node_ptr        annotated_node_ptr;
   typedef typename AnnotatedNodeTraits::const_annotated_node_ptr  const_annotated_node_ptr;


   static void update(annotated_node_ptr n){
      Annotation::template update_node<AnnotatedNodeTraits>(n);
/*      node_ptr left = node_traits::get_left(n);
      node_ptr right = node_traits::get_right(n);
      typename Annotation::type n_value = AnnotatedNodeTraits::get_annotation_input_value(AnnotatedNodeTraits::to_annotated_node_ptr(n));
      typename Annotation::type left_plus_value = left ?
          typename Annotation::operation()(AnnotatedNodeTraits::template get_annotation_value<Annotation>(AnnotatedNodeTraits::to_annotated_node_ptr(left)), n_value) :
          n_value;
      AnnotatedNodeTraits::template set_annotation_value<Annotation>(n, right ?
            typename Annotation::operation()(left_plus_value, AnnotatedNodeTraits::template get_annotation_value<Annotation>(AnnotatedNodeTraits::to_annotated_node_ptr(right))) :
            left_plus_value);*/
   }

   static void clone_node(const_annotated_node_ptr source, annotated_node_ptr target){
      AnnotatedNodeTraits::template set_annotation_value<Annotation>(target, AnnotatedNodeTraits::template get_annotation_value<Annotation>(source));
   }
};

template <class NodeTraits, class... Annotations>
struct annotation_list_algorithms;

/*
Specialize the no-annotation case to avoid iterating to the top of the tree for update_to_top
*/
template <class AnnotatedNodeTraits>
struct annotation_list_algorithms<AnnotatedNodeTraits>
{
   typedef typename AnnotatedNodeTraits::node_ptr        node_ptr;
   typedef typename AnnotatedNodeTraits::const_node_ptr  const_node_ptr;

   static void update(node_ptr){}
   static void clone_node(const_node_ptr source, node_ptr target){}
   static void update_to_top(node_ptr){}
   static void update_to_top(const_node_ptr, node_ptr){}
};

template <class AnnotatedNodeTraits, class AnnotationsHead, class... AnnotationsTail>
struct annotation_list_algorithms<AnnotatedNodeTraits, AnnotationsHead, AnnotationsTail...> : private annotation_list_algorithms<AnnotatedNodeTraits, AnnotationsTail...>
{
private:
   typedef annotation_list_algorithms<AnnotatedNodeTraits, AnnotationsTail...>  list_tail_algorithms;
   typedef typename AnnotatedNodeTraits::node_traits                            node_traits;
   typedef node_tree_algorithms<node_traits>                                    base_tree_algorithms;

public:
   typedef typename AnnotatedNodeTraits::node_ptr        node_ptr;
   typedef typename AnnotatedNodeTraits::const_node_ptr  const_node_ptr;

   static void update(node_ptr n){
      annotation_algorithms<AnnotatedNodeTraits, AnnotationsHead>::update(AnnotatedNodeTraits::to_annotated_node_ptr(n));
      list_tail_algorithms::update(n);
   };

   static void clone_node(const_node_ptr source, node_ptr target){
      annotation_algorithms<AnnotatedNodeTraits, AnnotationsHead>::clone_node(AnnotatedNodeTraits::to_annotated_node_ptr(source), AnnotatedNodeTraits::to_annotated_node_ptr(target));
      list_tail_algorithms::clone_node(source, target);
   }

   static void on_left_rotation(const_node_ptr pivot){
      update_to_top(pivot);
   }

   static void on_right_rotation(const_node_ptr pivot){
      update_to_top(pivot);
   }

   static void update_to_top(node_ptr n){
      while(!base_tree_algorithms::is_header(n)) {
         update(n);
         n = node_traits::get_parent(n);
      }
   }

   static void update_to_top(const_node_ptr header, node_ptr n){
      while(n != header) {
         update(n);
         n = node_traits::get_parent(n);
      }
   }
};

template <class AnnotatedNodeTraits, class AnnotationList>
struct make_annotation_list_algorithms
{
   typedef typename AnnotationList::template apply<
         annotation_list_algorithms,
         AnnotatedNodeTraits
      >::type type;
};

/*
BOOST_MPL_HAS_XXX_TRAIT_DEF(annotations);

template <class T>
struct eval_annotations
{
    typedef typename T::annotations type;
};

template <class T, class Default>
struct extract_annotations
{
    typedef typename std::conditional<has_annotations<T>::value, eval_annotations<T>, identity<Default>>::type::type::template apply<T>::type type;
};
*/

}
}
}

#endif // BOOST_INTRUSIVE_DETAIL_ANNOTATION_ALGORITHMS_HPP

// vim: set sw=3 sts=3 ts=3 et:
