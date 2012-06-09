#ifndef BOOST_INTRUSIVE_DETAIL_GENERIC_ANNOTATION_LIST_HPP
#define BOOST_INTRUSIVE_DETAIL_GENERIC_ANNOTATION_LIST_HPP

#include  <boost/intrusive/detail/pointer_to_other.hpp>
#include  <boost/intrusive/detail/utilities.hpp>

namespace boost {
namespace intrusive {
namespace detail {

template <class Annotation>
struct annotation_holder
{
   typename Annotation::type input_value;
   typename Annotation::type value;
};

template <class... Annotations>
struct generic_annotation_list;

template <>
struct generic_annotation_list<>
{ };

template <class Annotation, class... Annotations>
struct generic_annotation_list<Annotation, Annotations...> :
   public generic_annotation_list<Annotations...>,
   public annotation_holder<Annotation>
{ };

template <class Annotation>
typename Annotation::type get_annotation_value_helper(const annotation_holder<Annotation>* node)
{ return node->value; }

template <class Annotation>
void set_annotation_value_helper(annotation_holder<Annotation>* node, typename Annotation::type value)
{ node->value = value; }

template <class VoidPointer, class AnnotationsOpt, class... Annotations>
struct generic_annotation_list_traits_impl
{
   typedef AnnotationsOpt                          annotations;
   typedef generic_annotation_list<Annotations...> annotation_list;
   typedef typename boost::pointer_to_other<
      VoidPointer, annotation_list>::type          annotation_list_ptr;
   typedef typename boost::pointer_to_other<
      VoidPointer, const annotation_list>::type    const_annotation_list_ptr;

   template <class Annotation>
   static typename Annotation::type get_annotation_value(const_annotation_list_ptr n)
   { BOOST_STATIC_ASSERT_MSG((is_convertible<annotation_list*, annotation_holder<Annotation>*>::value),
         "get_annotation_value used for an unsupported annotation");
     return get_annotation_value_helper<Annotation>(::boost::intrusive::detail::boost_intrusive_get_pointer(n)); }

   template <class Annotation>
   static void set_annotation_value(annotation_list_ptr n, typename Annotation::type value)
   { BOOST_STATIC_ASSERT_MSG((is_convertible<annotation_list*, annotation_holder<Annotation>*>::value),
         "get_annotation_value used for an unsupported annotation");
     set_annotation_value_helper<Annotation>(::boost::intrusive::detail::boost_intrusive_get_pointer(n), value); }
};

template <class VoidPointer, class Annotations>
struct generic_annotation_list_traits :
   public Annotations::template apply<
         generic_annotation_list_traits_impl,
         VoidPointer,
         Annotations
      >::type
{ };

}
}
}

#endif // BOOST_INTRUSIVE_DETAIL_GENERIC_ANNOTATION_LIST_HPP
