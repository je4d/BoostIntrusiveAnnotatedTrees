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

#ifndef BOOST_INTRUSIVE_ANNOTATED_TREE_ALGORITHMS_HPP
#define BOOST_INTRUSIVE_ANNOTATED_TREE_ALGORITHMS_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/detail/assert.hpp>
#include <boost/intrusive/intrusive_fwd.hpp>
#include <cstddef>
#include <boost/intrusive/detail/utilities.hpp>
//iG pending #include <boost/pointer_cast.hpp>
#include <boost/intrusive/detail/node_tree_algorithms.hpp>
#include <boost/intrusive/detail/annotation_algorithms.hpp>
#include <boost/intrusive/options.hpp>

namespace boost {
namespace intrusive {
namespace detail {

template<class AnnotatedNodeTraits, class Annotations = ::boost::intrusive::annotations<> >
class annotated_tree_algorithms : public node_tree_algorithms<typename AnnotatedNodeTraits::node_traits>
{
   public:
   typedef typename AnnotatedNodeTraits::node_traits  node_traits;
   typedef typename node_traits::node                 node;
   typedef typename node_traits::node_ptr             node_ptr;
   typedef typename node_traits::const_node_ptr       const_node_ptr;
   typedef node_tree_algorithms<node_traits>          base_type;
   typedef typename base_type::insert_commit_data     insert_commit_data;
   typedef typename base_type::data_for_rebalance     data_for_rebalance;
   typedef typename make_annotated_node_algorithms<
      AnnotatedNodeTraits, Annotations>::type         annotation_algorithms;

   private:
   struct nop_erase_fixup
   {
      void operator()(node_ptr, node_ptr){}
   };

   public:
   static void swap_nodes(node_ptr node1, node_ptr node2)
   {
      if(node1 == node2)
         return;

      node_ptr header1(get_header(node1)), header2(get_header(node2));
      swap_nodes(node1, header1, node2, header2);
   }

   static void swap_nodes(node_ptr node1, node_ptr header1, node_ptr node2, node_ptr header2)
   {
      if(node1 == node2)
         return;
      base_type::swap_nodes(node1, header1, node2, header2);
      annotation_algorithms::update_to_top(header2, node1);
      annotation_algorithms::update_to_top(header1, node2);
   }

   static void replace_node(node_ptr node_to_be_replaced, node_ptr new_node)
   {
      if(node_to_be_replaced == new_node)
         return;
      replace_node(node_to_be_replaced, get_header(node_to_be_replaced), new_node);
   }

   static void replace_node(node_ptr node_to_be_replaced, node_ptr header, node_ptr new_node)
   {
      if(node_to_be_replaced == new_node)
         return;
      base_type::replace_node(node_to_be_replaced, header, new_node);
      annotation_algorithms::update_to_top(header, new_node);
   }

   static void insert_unique_commit
      (node_ptr header, node_ptr new_value, const insert_commit_data &commit_data)
   {  return insert_commit(header, new_value, commit_data); }

   static void insert_commit
      (node_ptr header, node_ptr new_node, const insert_commit_data &commit_data)
   {
      base_type::insert_commit(header, new_node, commit_data);
      annotation_algorithms::update_to_top(header, new_node);
   }

   template<class NodePtrCompare>
   static node_ptr insert_equal
      (const node_ptr & h, const node_ptr & hint, const node_ptr & new_node, NodePtrCompare comp, std::size_t *pdepth = 0)
   {
      insert_commit_data commit_data;
      base_type::insert_equal_check(h, hint, new_node, comp, commit_data, pdepth);
      insert_commit(h, new_node, commit_data);
      return new_node;
   }

   template<class NodePtrCompare>
   static node_ptr insert_equal_upper_bound
      (const node_ptr & h, const node_ptr & new_node, NodePtrCompare comp, std::size_t *pdepth = 0)
   {
      insert_commit_data commit_data;
      base_type::insert_equal_upper_bound_check(h, new_node, comp, commit_data, pdepth);
      insert_commit(h, new_node, commit_data);
      return new_node;
   }

   template<class NodePtrCompare>
   static node_ptr insert_equal_lower_bound
      (const node_ptr & h, const node_ptr & new_node, NodePtrCompare comp, std::size_t *pdepth = 0)
   {
      insert_commit_data commit_data;
      base_type::insert_equal_lower_bound_check(h, new_node, comp, commit_data, pdepth);
      insert_commit(h, new_node, commit_data);
      return new_node;
   }

   static node_ptr insert_before
      (const node_ptr & header, const node_ptr & pos, const node_ptr & new_node, std::size_t *pdepth = 0)
   {
      insert_commit_data commit_data;
      base_type::insert_before_check(header, pos, commit_data, pdepth);
      insert_commit(header, new_node, commit_data);
      return new_node;
   }

   static void push_back
      (const node_ptr & header, const node_ptr & new_node, std::size_t *pdepth = 0)
   {
      insert_commit_data commit_data;
      base_type::push_back_check(header, commit_data, pdepth);
      insert_commit(header, new_node, commit_data);
   }

   static void push_front
      (const node_ptr & header, const node_ptr & new_node, std::size_t *pdepth = 0)
   {
      insert_commit_data commit_data;
      base_type::push_front_check(header, commit_data, pdepth);
      insert_commit(header, new_node, commit_data);
   }

   template <class Cloner>
   struct clone_annotations_cloner
   {
      clone_annotations_cloner(Cloner& cloner) : c(cloner) {}

      node_ptr operator()(node_ptr n) {
         node_ptr new_node = c(n);
         annotation_algorithms::clone_node(n, new_node);
         return new_node;
      }
      Cloner& c;
   };

   template <class Cloner, class Disposer>
   static void clone
      (const_node_ptr source_header, node_ptr target_header, Cloner cloner, Disposer disposer)
   {
      clone_annotations_cloner<Cloner> cloner2(cloner);
      base_type::clone(source_header, target_header, cloner2, disposer);
   }

   template <class Cloner, class Disposer>
   static node_ptr clone_subtree
      ( const_node_ptr source_parent,  node_ptr target_parent
      , Cloner cloner,                 Disposer disposer
      , node_ptr &leftmost_out,        node_ptr &rightmost_out
      )
   {
      clone_annotations_cloner<Cloner> cloner2(cloner);
      return base_type::clone_subtree(source_parent, target_parent, cloner2, disposer, leftmost_out, rightmost_out);
   }

   // rotate parent p to left (no header and p's parent fixup)
   static node_ptr rotate_left(node_ptr p)
   {
      node_ptr x = base_type::rotate_left(p);
      annotation_algorithms::on_left_rotation(p);
      return x;
   }

   // rotate parent p to left (with header and p's parent fixup)
   static void rotate_left(node_ptr p, node_ptr header)
   {
      bool     p_was_left(is_left_child(p));
      node_ptr p_old_parent(node_traits::get_parent(p));
      node_ptr x(rotate_left(p));
      node_traits::set_parent(x, p_old_parent);
      base_type::replace_own_impl(p, x, header, p_old_parent, p_was_left);
   }

   // rotate parent p to right (no header and p's parent fixup)
   static node_ptr rotate_right(node_ptr p)
   {
      node_ptr x = base_type::rotate_right(p);
      annotation_algorithms::on_right_rotation(p);
      return x;
   }

   // rotate parent p to right (with header and p's parent fixup)
   static void rotate_right(node_ptr p, node_ptr header)
   {
      bool     p_was_left(is_left_child(p));
      node_ptr p_old_parent(node_traits::get_parent(p));
      node_ptr x(rotate_right(p));
      node_traits::set_parent(x, p_old_parent);
      replace_own_impl(p, x, header, p_old_parent, p_was_left);
   }

   static void erase(const node_ptr & header, const node_ptr & z)
   {
      data_for_rebalance ignored;
      erase(header, z, nop_erase_fixup(), ignored);
   }

   template<class F>
   static void erase(const node_ptr & header, const node_ptr & z, F z_and_successor_fixup, data_for_rebalance &info)
   {
      base_type::erase(header, z, z_and_successor_fixup, info);
      annotation_algorithms::update_to_top(header, info.x_parent);
   }

   static void unlink(const node_ptr & node)
   {
      node_ptr x = node_traits::get_parent(node);
      if(x){
         while(!is_header(x))
            x = node_traits::get_parent(x);
         erase(x, node);
      }
   }
};

}  //namespace detail {
}  //namespace intrusive 
}  //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_ANNOTATED_TREE_ALGORITHMS_HPP
// vim: set sw=3 sts=3 ts=3 et:
