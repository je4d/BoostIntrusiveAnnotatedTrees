#ifndef BOOST_INTRUSIVE_DETAIL_ANNOTATION_LIST_HPP
#define BOOST_INTRUSIVE_DETAIL_ANNOTATION_LIST_HPP

namespace boost {
namespace intrusive {
namespace detail {

template <class NodeTraits, class AnnotationListTraits>
struct generic_annotated_node :
   public NodeTraits::node,
   public AnnotationListTraits::annotation_list
{};

template <class NodeTraits, class AnnotationListTraits>
struct generic_annotated_node_traits
{
   typedef NodeTraits                              node_traits;
   typedef typename NodeTraits::node               node;
   typedef typename NodeTraits::node_ptr           node_ptr;
   typedef typename NodeTraits::const_node_ptr     const_node_ptr;

   typedef AnnotationListTraits                    annotation_list_traits;
   typedef typename AnnotationListTraits::annotation_list
                                                   annotation_list;
   typedef typename boost::pointer_to_other<
      node_ptr, annotation_list>::type             annotation_list_ptr;
   typedef typename boost::pointer_to_other<
      node_ptr, const annotation_list>::type       const_annotation_list_ptr;

   typedef generic_annotated_node<
      NodeTraits, AnnotationListTraits>            annotated_node;
   typedef typename boost::pointer_to_other<
      node_ptr, annotated_node>::type              annotated_node_ptr;
   typedef typename boost::pointer_to_other<
      node_ptr, const annotated_node>::type        const_annotated_node_ptr;


   static node_ptr to_node_ptr(annotation_list_ptr an)
   {
      return node_ptr(
            static_cast<node*>(
               static_cast<annotated_node*>(
                  ::boost::intrusive::detail::boost_intrusive_get_pointer(an))));
   }

   static const_node_ptr to_node_ptr(const_annotation_list_ptr an)
   {
      return const_node_ptr(
            static_cast<const node*>(
               static_cast<const annotated_node*>(
                  ::boost::intrusive::detail::boost_intrusive_get_pointer(an))));
   }

   static annotation_list_ptr to_annotation_list_ptr (node_ptr n)
   {
      return annotation_list_ptr(
            static_cast<annotation_list*>(
               static_cast<annotated_node*>(
                  ::boost::intrusive::detail::boost_intrusive_get_pointer(n))));
   }

   static const_annotation_list_ptr  to_annotation_list_ptr (const_node_ptr n)
   {
      return const_annotation_list_ptr(
            static_cast<const annotation_list*>(
               static_cast<const annotated_node*>(
                  ::boost::intrusive::detail::boost_intrusive_get_pointer(n))));
   }
};

}
}
}

#endif // BOOST_INTRUSIVE_DETAIL_ANNOTATION_LIST_HPP
