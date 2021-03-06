
//  (C) Copyright Edward Diener 2011,2012
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(BOOST_TTI_DETAIL_MEM_FUN_HPP)
#define BOOST_TTI_DETAIL_MEM_FUN_HPP

#include <boost/config.hpp>
#include <boost/function_types/is_member_function_pointer.hpp>
#include <boost/function_types/property_tags.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/tti/detail/dcomp_mem_fun.hpp>
#include <boost/tti/detail/ddeftype.hpp>
#include <boost/tti/detail/dnullptr.hpp>
#include <boost/tti/detail/dptmf.hpp>
#include <boost/tti/gen/namespace_gen.hpp>

#define BOOST_TTI_DETAIL_TRAIT_HAS_TYPES_MEMBER_FUNCTION(trait,name) \
  template<class T,class C> \
  struct BOOST_PP_CAT(trait,_detail_types) \
    { \
    template<T> \
    struct helper; \
    \
    template<class U> \
    static ::boost::type_traits::yes_type check(helper<&U::name> *); \
    \
    template<class U> \
    static ::boost::type_traits::no_type check(...); \
    \
    BOOST_STATIC_CONSTANT(bool,value=sizeof(check<C>(BOOST_TTI_DETAIL_NULLPTR))==sizeof(::boost::type_traits::yes_type)); \
    \
    typedef boost::mpl::bool_<value> type; \
    }; \
/**/

#define BOOST_TTI_DETAIL_TRAIT_HAS_CALL_TYPES_MEMBER_FUNCTION(trait,name) \
  BOOST_TTI_DETAIL_TRAIT_HAS_TYPES_MEMBER_FUNCTION(trait,name) \
  template<class TTI_T,class TTI_R,class TTI_FS,class TTI_TAG> \
  struct BOOST_PP_CAT(trait,_detail_call_types) : \
    BOOST_PP_CAT(trait,_detail_types) \
      < \
      typename BOOST_TTI_NAMESPACE::detail::ptmf_seq<TTI_T,TTI_R,TTI_FS,TTI_TAG>::type, \
      typename boost::remove_const<TTI_T>::type \
      > \
    { \
    }; \
/**/

#define BOOST_TTI_DETAIL_TRAIT_CHECK_HAS_COMP_MEMBER_FUNCTION(trait,name) \
  BOOST_TTI_DETAIL_TRAIT_HAS_COMP_MEMBER_FUNCTION(trait,name) \
  template<class TTI_T> \
  struct BOOST_PP_CAT(trait,_detail_check_comp) : \
    BOOST_PP_CAT(trait,_detail_hcmf)<TTI_T> \
    { \
    BOOST_MPL_ASSERT((boost::function_types::is_member_function_pointer<TTI_T>)); \
    }; \
/**/

#define BOOST_TTI_DETAIL_TRAIT_HAS_MEMBER_FUNCTION(trait,name) \
  BOOST_TTI_DETAIL_TRAIT_HAS_CALL_TYPES_MEMBER_FUNCTION(trait,name) \
  BOOST_TTI_DETAIL_TRAIT_CHECK_HAS_COMP_MEMBER_FUNCTION(trait,name) \
  template<class TTI_T,class TTI_R,class TTI_FS,class TTI_TAG> \
  struct BOOST_PP_CAT(trait,_detail_hmf) \
    { \
    typedef typename \
    boost::mpl::eval_if \
      < \
      boost::mpl::and_ \
        < \
        boost::is_same<TTI_R,BOOST_TTI_NAMESPACE::detail::deftype>, \
        boost::is_same<TTI_FS,boost::mpl::vector<> >, \
        boost::is_same<TTI_TAG,boost::function_types::null_tag> \
        >, \
      BOOST_PP_CAT(trait,_detail_check_comp)<TTI_T>, \
      BOOST_PP_CAT(trait,_detail_call_types)<TTI_T,TTI_R,TTI_FS,TTI_TAG> \
      >::type type; \
    \
    BOOST_STATIC_CONSTANT(bool,value=type::value); \
    }; \
/**/

#endif // BOOST_TTI_DETAIL_MEM_FUN_HPP
