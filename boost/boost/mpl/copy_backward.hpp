//-----------------------------------------------------------------------------
// boost mpl/copy_backward.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_COPY_BACKWARD_HPP_INCLUDED
#define BOOST_MPL_COPY_BACKWARD_HPP_INCLUDED

#include "boost/mpl/fold_backward.hpp"
#include "boost/mpl/aux_/copy_op.hpp"
#include "boost/mpl/lambda.hpp"
#include "boost/mpl/protect.hpp"
#include "boost/mpl/aux_/void_spec.hpp"

namespace boost {
namespace mpl {

template<
      typename InputSequence
    , typename Operation
    , typename OutputSequence
    >
struct copy_backward
{
 private:
    typedef typename lambda<Operation>::type op_;

 public:
    typedef typename fold_backward<
          InputSequence
        , OutputSequence
        , protect< aux::copy_op<op_> >
        >::type type;
};

BOOST_MPL_AUX_VOID_SPEC(3, copy_backward)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_COPY_BACKWARD_HPP_INCLUDED
