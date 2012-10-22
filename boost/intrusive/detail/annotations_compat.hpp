/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Jeff Snyder    2012
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_DETAIL_ANNOTATIONS_COMPAT_HPP
#define BOOST_INTRUSIVE_DETAIL_ANNOTATIONS_COMPAT_HPP

#if !defined(BOOST_INTRUSIVE_VARIADIC_TEMPLATES)

// TAT = Template argument Types
#define BOOST_INTRUSIVE_ANNOTATION_TAT class,class,class,class,class,class,class,class,class,class
// TANN = Template argument Types and Names
#define BOOST_INTRUSIVE_ANNOTATION_TATN class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9, class A10
// TANND = Template argument Types and Names w/Defaults
#define BOOST_INTRUSIVE_ANNOTATION_TATND class A1 = no_annotation, class A2 = no_annotation, class A3 = no_annotation, class A4 = no_annotation, class A5 = no_annotation, class A6 = no_annotation, class A7 = no_annotation, class A8 = no_annotation, class A9 = no_annotation, class A10 = no_annotation
// TAD = Template Argument Defaults
#define BOOST_INTRUSIVE_ANNOTATION_TAD no_annotation,no_annotation,no_annotation,no_annotation,no_annotation,no_annotation,no_annotation,no_annotation,no_annotation,no_annotation
// TAN = Template Argument Names
#define BOOST_INTRUSIVE_ANNOTATION_TAN A1,A2,A3,A4,A5,A6,A7,A8,A9,A10
// TAN = Template Argument Names, first shifted off
#define BOOST_INTRUSIVE_ANNOTATION_TAN_SHIFT A2,A3,A4,A5,A6,A7,A8,A9,A10

namespace boost {
namespace intrusive {
struct no_annotation;
} //namespace intrusive
} //namespace boost

#endif

#endif // BOOST_INTRUSIVE_DETAIL_ANNOTATIONS_COMPAT_HPP

