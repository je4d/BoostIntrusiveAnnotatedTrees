/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    http://www.boost.org/

    Copyright (c) 2001-2006 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(CPP_EXPRESSION_GRAMMAR_HPP_099CD1A4_A6C0_44BE_8F24_0B00F5BE5674_INCLUDED)
#define CPP_EXPRESSION_GRAMMAR_HPP_099CD1A4_A6C0_44BE_8F24_0B00F5BE5674_INCLUDED

#include <boost/assert.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute/closure.hpp>
#include <boost/spirit/dynamic/if.hpp>
#if SPIRIT_VERSION >= 0x1700
#include <boost/spirit/actor/assign_actor.hpp>
#include <boost/spirit/actor/push_back_actor.hpp>
#endif // SPIRIT_VERSION >= 0x1700

#include <boost/spirit/phoenix/functions.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/statements.hpp>
#include <boost/spirit/phoenix/casts.hpp>

#include <boost/wave/wave_config.hpp>
#include <boost/wave/token_ids.hpp>

#include <boost/wave/cpp_exceptions.hpp>
#include <boost/wave/grammars/cpp_expression_grammar_gen.hpp>   
#include <boost/wave/grammars/cpp_literal_grammar_gen.hpp>  
#include <boost/wave/grammars/cpp_expression_value.hpp>
#include <boost/wave/util/pattern_parser.hpp>
#include <boost/wave/util/macro_helpers.hpp>

#if !defined(spirit_append_actor)
#if SPIRIT_VERSION >= 0x1700
#define spirit_append_actor(actor) boost::spirit::push_back_a(actor)
#define spirit_assign_actor(actor) boost::spirit::assign_a(actor)
#else
#define spirit_append_actor(actor) boost::spirit::append(actor)
#define spirit_assign_actor(actor) boost::spirit::assign(actor)
#endif // SPIRIT_VERSION >= 0x1700
#endif // !defined(spirit_append_actor)

// this must occur after all of the includes and before any code appears
#ifdef BOOST_HAS_ABI_HEADERS
#include BOOST_ABI_PREFIX
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Encapsulation of the grammar for evaluation of constant preprocessor
//  expressions
//
///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave { 
namespace grammars {
namespace closures {

///////////////////////////////////////////////////////////////////////////////
//
//  define the closure type used throughout the C++ expression grammar
//
//      Throughout this grammar all literal tokens are stored into a 
//      closure_value variables, which converts the types appropriately, where 
//      required.
//
///////////////////////////////////////////////////////////////////////////////
    struct cpp_expr_closure 
    :   boost::spirit::closure<cpp_expr_closure, closure_value> 
    {
        member1 val;
    };

}   // namespace closures

namespace impl {

///////////////////////////////////////////////////////////////////////////////
//
//  convert the given tokenvalue (integer literal) to a unsigned long
//
///////////////////////////////////////////////////////////////////////////////
    struct convert_intlit {

        template <typename ArgT>
        struct result { 
        
            typedef boost::wave::grammars::closures::closure_value type; 
        };

        template <typename TokenT>
        boost::wave::grammars::closures::closure_value 
        operator()(TokenT const &token) const
        { 
            typedef boost::wave::grammars::closures::closure_value return_t;
            bool is_unsigned = false;
            unsigned long ul = intlit_grammar_gen<TokenT>::evaluate(token, 
                is_unsigned);

            return is_unsigned ? return_t(ul) : return_t(static_cast<long>(ul));
        }
    };
    phoenix::function<convert_intlit> const as_intlit;

///////////////////////////////////////////////////////////////////////////////
//
//  convert the given tokenvalue (character literal) to a unsigned int
//
///////////////////////////////////////////////////////////////////////////////
    struct convert_chlit {

        template <typename ArgT>
        struct result { 
        
            typedef boost::wave::grammars::closures::closure_value type; 
        };

        template <typename TokenT>
        boost::wave::grammars::closures::closure_value 
        operator()(TokenT const &token) const
        { 
            typedef boost::wave::grammars::closures::closure_value return_t;
            return return_t(chlit_grammar_gen<TokenT>::evaluate(token));
        }
    };
    phoenix::function<convert_chlit> const as_chlit;

////////////////////////////////////////////////////////////////////////////////
//
//  Handle the ?: operator with correct type propagation
//
////////////////////////////////////////////////////////////////////////////////
    struct operator_questionmark {
    
        template <typename CondT, typename Arg1T, typename Arg2T>
        struct result { 
        
            typedef boost::wave::grammars::closures::closure_value type; 
        };

        template <typename CondT, typename Arg1T, typename Arg2T>
        boost::wave::grammars::closures::closure_value 
        operator()(CondT const &cond, Arg1T &val1, Arg2T const &val2) const
        { 
            typedef boost::wave::grammars::closures::closure_value return_t;
            return return_t(val1.handle_questionmark(cond, val2));
        }
    };
    phoenix::function<operator_questionmark> const questionmark;
    
}   // namespace impl

///////////////////////////////////////////////////////////////////////////////
//  define, whether the rule's should generate some debug output
#define TRACE_CPP_EXPR_GRAMMAR \
    bool(BOOST_SPIRIT_DEBUG_FLAGS_CPP & BOOST_SPIRIT_DEBUG_FLAGS_CPP_EXPR_GRAMMAR) \
    /**/

struct expression_grammar :
    public boost::spirit::grammar<
        expression_grammar, 
        closures::cpp_expr_closure::context_t
    >
{
    expression_grammar()
    {
        BOOST_SPIRIT_DEBUG_TRACE_GRAMMAR_NAME(*this, "expression_grammar", 
            TRACE_CPP_EXPR_GRAMMAR);
    }
    
    template <typename ScannerT>
    struct definition
    {
        typedef closures::cpp_expr_closure closure_type;
        typedef boost::spirit::rule<ScannerT, closure_type::context_t> rule_t;
        typedef boost::spirit::rule<ScannerT> simple_rule_t;

        simple_rule_t pp_expression;
        
        rule_t const_exp;
        rule_t logical_or_exp, logical_and_exp;
        rule_t inclusive_or_exp, exclusive_or_exp, and_exp;
        rule_t cmp_equality, cmp_relational;
        rule_t shift_exp;
        rule_t add_exp, multiply_exp;
        rule_t unary_exp, primary_exp, constant;

        rule_t const_exp_nocalc;
        rule_t logical_or_exp_nocalc, logical_and_exp_nocalc;
        rule_t inclusive_or_exp_nocalc, exclusive_or_exp_nocalc, and_exp_nocalc;
        rule_t cmp_equality_nocalc, cmp_relational_nocalc;
        rule_t shift_exp_nocalc;
        rule_t add_exp_nocalc, multiply_exp_nocalc;
        rule_t unary_exp_nocalc, primary_exp_nocalc, constant_nocalc;

        boost::spirit::subrule<0, closure_type::context_t> const_exp_subrule;

        definition(expression_grammar const &self)
        {
            using namespace boost::spirit;
            using namespace phoenix;
            using namespace boost::wave;
            using boost::wave::util::pattern_p;
            
            pp_expression
                =   const_exp[self.val = arg1]
                ;
                
            const_exp
                =   logical_or_exp[const_exp.val = arg1]
                    >> !(const_exp_subrule =
                            ch_p(T_QUESTION_MARK)
                            >>  const_exp
                                [
                                    const_exp_subrule.val = arg1
                                ] 
                            >>  ch_p(T_COLON)
                            >>  const_exp
                                [
                                    const_exp_subrule.val = 
                                        impl::questionmark(const_exp.val, 
                                            const_exp_subrule.val, arg1)
                                ]
                        )[const_exp.val = arg1]
                ;

            logical_or_exp 
                =   logical_and_exp[logical_or_exp.val = arg1]
                    >> *(   if_p(static_cast_<bool>(logical_or_exp.val))
                            [
                                // if one of the || operators is true, no more
                                // evaluation is required
                                pattern_p(T_OROR, MainTokenMask)
                                >>  logical_and_exp_nocalc
                                    [
                                        logical_or_exp.val = 
                                            static_cast_<bool>(logical_or_exp.val)
                                    ]
                            ]
                            .else_p
                            [
                                pattern_p(T_OROR, MainTokenMask)
                                >>  logical_and_exp
                                    [
                                        logical_or_exp.val = 
                                            logical_or_exp.val || arg1
                                    ]
                            ]
                        )
                ;

            logical_and_exp
                =   inclusive_or_exp[logical_and_exp.val = arg1]
                    >> *(   if_p(static_cast_<bool>(logical_and_exp.val))
                            [
                                pattern_p(T_ANDAND, MainTokenMask)
                                >>  inclusive_or_exp
                                    [
                                        logical_and_exp.val = 
                                            logical_and_exp.val && arg1
                                    ]
                            ]
                            .else_p
                            [
                                // if one of the && operators is false, no more
                                // evaluation is required
                                pattern_p(T_ANDAND, MainTokenMask)
                                >>  inclusive_or_exp_nocalc
                                    [
                                        logical_and_exp.val =
                                            static_cast_<bool>(logical_and_exp.val)
                                    ]
                            ]
                        )
                ;

            inclusive_or_exp
                =   exclusive_or_exp[inclusive_or_exp.val = arg1]
                    >> *(   pattern_p(T_OR, MainTokenMask)
                            >>  exclusive_or_exp
                                [
                                    inclusive_or_exp.val = 
                                          static_cast_<unsigned int>(inclusive_or_exp.val) 
                                      |   static_cast_<unsigned int>(arg1)
                                ]
                        )
                ;

            exclusive_or_exp
                =   and_exp[exclusive_or_exp.val = arg1]
                    >> *(   pattern_p(T_XOR, MainTokenMask)
                            >>  and_exp
                                [
                                    exclusive_or_exp.val = 
                                          static_cast_<unsigned int>(exclusive_or_exp.val)
                                      ^   static_cast_<unsigned int>(arg1)
                                ]
                        )
                ;

            and_exp
                =   cmp_equality[and_exp.val = arg1]
                    >> *(   pattern_p(T_AND, MainTokenMask)
                            >>  cmp_equality
                                [
                                    and_exp.val = 
                                          static_cast_<unsigned int>(and_exp.val)
                                      &   static_cast_<unsigned int>(arg1)
                                ]
                        )
                ;

            cmp_equality
                =   cmp_relational[cmp_equality.val = arg1]
                    >> *(   ch_p(T_EQUAL)
                            >>  cmp_relational
                                [
                                    cmp_equality.val = 
                                        cmp_equality.val == arg1
                                ]
                        |   pattern_p(T_NOTEQUAL, MainTokenMask)
                            >>  cmp_relational
                                [
                                    cmp_equality.val = 
                                        cmp_equality.val != arg1
                                ]
                        )
                ;

            cmp_relational
                =   shift_exp[cmp_relational.val = arg1]
                    >> *(   ch_p(T_LESSEQUAL)
                            >>  shift_exp
                                [
                                    cmp_relational.val = 
                                        cmp_relational.val <= arg1
                                ]
                        |   ch_p(T_GREATEREQUAL)
                            >>  shift_exp
                                [
                                    cmp_relational.val = 
                                        cmp_relational.val >= arg1
                                ]
                        |   ch_p(T_LESS)
                            >>  shift_exp
                                [
                                    cmp_relational.val = 
                                        cmp_relational.val < arg1
                                ]
                        |   ch_p(T_GREATER)
                            >>  shift_exp
                                [
                                    cmp_relational.val = 
                                        cmp_relational.val > arg1
                                ]
                        )
                ;

            shift_exp
                =   add_exp[shift_exp.val = arg1]
                    >> *(   ch_p(T_SHIFTLEFT)
                            >>  add_exp
                                [
                                    shift_exp.val <<= arg1
                                ]
                        |   ch_p(T_SHIFTRIGHT)
                            >>  add_exp
                                [
                                    shift_exp.val >>= arg1
                                ]
                        )
                ;

            add_exp
                =   multiply_exp[add_exp.val = arg1]
                    >> *(   ch_p(T_PLUS)
                            >>  multiply_exp
                                [
                                    add_exp.val += arg1
                                ]
                        |   ch_p(T_MINUS)
                            >>  multiply_exp
                                [
                                    add_exp.val -= arg1
                                ]
                        )
                ;

            multiply_exp
                =   unary_exp[multiply_exp.val = arg1]
                    >> *(   ch_p(T_STAR)
                            >>  unary_exp
                                [
                                    multiply_exp.val *= arg1
                                ]
                        |   ch_p(T_DIVIDE)
                            >>  unary_exp
                                [
                                    multiply_exp.val /= arg1
                                ]
                        |   ch_p(T_PERCENT)
                            >>  unary_exp
                                [
                                    multiply_exp.val %= arg1
                                ]
                        )
                ;

            unary_exp
                =   primary_exp[unary_exp.val = arg1]
                |   ch_p(T_PLUS) >> unary_exp
                    [
                        unary_exp.val = arg1
                    ]
                |   ch_p(T_MINUS) >> unary_exp
                    [
                        unary_exp.val = -arg1
                    ]
                |   pattern_p(T_COMPL, MainTokenMask) >> unary_exp
                    [
                        unary_exp.val = ~arg1
                    ]
                |   pattern_p(T_NOT, MainTokenMask) >> unary_exp
                    [
                        unary_exp.val = !arg1
                    ]
                ;

            primary_exp
                =   constant[primary_exp.val = arg1]
                |   ch_p(T_LEFTPAREN) 
                    >> const_exp[primary_exp.val = arg1]
                    >> ch_p(T_RIGHTPAREN)
                ;

            constant
                =   ch_p(T_INTLIT) 
                    [
                        constant.val = impl::as_intlit(arg1)
                    ]
                |   ch_p(T_CHARLIT) 
                    [
                        constant.val = impl::as_chlit(arg1)
                    ]
                ;
              
            //  here follows the same grammar, but without any embedded 
            //  calculations
            const_exp_nocalc
                =   logical_or_exp_nocalc
                    >> !(   ch_p(T_QUESTION_MARK)
                            >>  const_exp_nocalc
                            >>  ch_p(T_COLON)
                            >>  const_exp_nocalc
                        )
                ;

            logical_or_exp_nocalc 
                =   logical_and_exp_nocalc
                    >> *(   pattern_p(T_OROR, MainTokenMask)
                            >>  logical_and_exp_nocalc
                        )
                ;

            logical_and_exp_nocalc
                =   inclusive_or_exp_nocalc
                    >> *(   pattern_p(T_ANDAND, MainTokenMask)
                            >>  inclusive_or_exp_nocalc
                        )
                ;

            inclusive_or_exp_nocalc
                =   exclusive_or_exp_nocalc
                    >> *(   pattern_p(T_OR, MainTokenMask)
                            >>  exclusive_or_exp_nocalc
                        )
                ;

            exclusive_or_exp_nocalc
                =   and_exp_nocalc
                    >> *(   pattern_p(T_XOR, MainTokenMask)
                            >>  and_exp_nocalc
                        )
                ;

            and_exp_nocalc
                =   cmp_equality_nocalc
                    >> *(   pattern_p(T_AND, MainTokenMask)
                            >>  cmp_equality_nocalc
                        )
                ;

            cmp_equality_nocalc
                =   cmp_relational_nocalc
                    >> *(   ch_p(T_EQUAL)
                            >>  cmp_relational_nocalc
                        |   pattern_p(T_NOTEQUAL, MainTokenMask)
                            >>  cmp_relational_nocalc
                        )
                ;

            cmp_relational_nocalc
                =   shift_exp_nocalc
                    >> *(   ch_p(T_LESSEQUAL)
                            >>  shift_exp_nocalc
                        |   ch_p(T_GREATEREQUAL)
                            >>  shift_exp_nocalc
                        |   ch_p(T_LESS)
                            >>  shift_exp_nocalc
                        |   ch_p(T_GREATER)
                            >>  shift_exp_nocalc
                        )
                ;

            shift_exp_nocalc
                =   add_exp_nocalc
                    >> *(   ch_p(T_SHIFTLEFT)
                            >>  add_exp_nocalc
                        |   ch_p(T_SHIFTRIGHT)
                            >>  add_exp_nocalc
                        )
                ;

            add_exp_nocalc
                =   multiply_exp_nocalc
                    >> *(   ch_p(T_PLUS)
                            >>  multiply_exp_nocalc
                        |   ch_p(T_MINUS)
                            >>  multiply_exp_nocalc
                        )
                ;

            multiply_exp_nocalc
                =   unary_exp_nocalc
                    >> *(   ch_p(T_STAR)
                            >>  unary_exp_nocalc
                        |   ch_p(T_DIVIDE)
                            >>  unary_exp_nocalc
                        |   ch_p(T_PERCENT)
                            >>  unary_exp_nocalc
                        )
                ;

            unary_exp_nocalc
                =   primary_exp_nocalc
                |   ch_p(T_PLUS) >> unary_exp_nocalc
                |   ch_p(T_MINUS) >> unary_exp_nocalc
                |   pattern_p(T_COMPL, MainTokenMask) >> unary_exp_nocalc
                |   pattern_p(T_NOT, MainTokenMask) >> unary_exp_nocalc
                ;

            primary_exp_nocalc
                =   constant_nocalc
                |   ch_p(T_LEFTPAREN) 
                    >> const_exp_nocalc
                    >> ch_p(T_RIGHTPAREN)
                ;

            constant_nocalc
                =   ch_p(T_INTLIT) 
                |   ch_p(T_CHARLIT) 
                ;

            BOOST_SPIRIT_DEBUG_TRACE_RULE(pp_expression, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(const_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(logical_or_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(logical_and_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(inclusive_or_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(exclusive_or_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(and_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(cmp_equality, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(cmp_relational, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(shift_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(add_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(multiply_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(unary_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(primary_exp, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(constant, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(const_exp_subrule, TRACE_CPP_EXPR_GRAMMAR);

            BOOST_SPIRIT_DEBUG_TRACE_RULE(const_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(logical_or_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(logical_and_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(inclusive_or_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(exclusive_or_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(and_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(cmp_equality_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(cmp_relational_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(shift_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(add_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(multiply_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(unary_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(primary_exp_nocalc, TRACE_CPP_EXPR_GRAMMAR);
            BOOST_SPIRIT_DEBUG_TRACE_RULE(constant_nocalc, TRACE_CPP_EXPR_GRAMMAR);
        }

    // start rule of this grammar
        simple_rule_t const& start() const
        { return pp_expression; }
    };
};

///////////////////////////////////////////////////////////////////////////////
#undef TRACE_CPP_EXPR_GRAMMAR

///////////////////////////////////////////////////////////////////////////////
//  
//  The following function is defined here, to allow the separation of 
//  the compilation of the expression_grammar from the function using it.
//  
///////////////////////////////////////////////////////////////////////////////

#if BOOST_WAVE_SEPARATE_GRAMMAR_INSTANTIATION != 0
#define BOOST_WAVE_EXPRGRAMMAR_GEN_INLINE
#else
#define BOOST_WAVE_EXPRGRAMMAR_GEN_INLINE inline
#endif 

template <typename TokenT>
BOOST_WAVE_EXPRGRAMMAR_GEN_INLINE 
bool 
expression_grammar_gen<TokenT>::evaluate(
    typename token_sequence_type::const_iterator const &first, 
    typename token_sequence_type::const_iterator const &last, 
    typename token_type::position_type const &act_pos,
    bool if_block_status)
{
    using namespace boost::spirit;
    using namespace boost::wave;
    using namespace boost::wave::grammars::closures;
    
    using boost::wave::util::impl::as_string;
    
    typedef typename token_sequence_type::const_iterator iterator_type;
    typedef typename token_sequence_type::value_type::string_type string_type;

    parse_info<iterator_type> hit(first);
    closure_value result;             // expression result
    
    try {
        expression_grammar g;             // expression grammar
        hit = parse (first, last, g[spirit_assign_actor(result)], 
                     ch_p(T_SPACE) | ch_p(T_CCOMMENT) | ch_p(T_CPPCOMMENT));

        if (!hit.hit) {
        // expression is illformed
            if (if_block_status) {
                string_type expression = as_string<string_type>(first, last);
                if (0 == expression.size()) 
                    expression = "empty expression";
                BOOST_WAVE_THROW(preprocess_exception, ill_formed_expression, 
                    expression.c_str(), act_pos);
            }
            else {
            //  as the if_block_status is false no errors will be reported
                return false;
            }
        }
    }
    catch (wave::preprocess_exception const& e) {
    // expression is illformed
        if (if_block_status) {
            boost::throw_exception(e);
        }
        else {
        //  as the if_block_status is false no errors will be reported
            return false;
        }
    }
        
    if (!hit.full) {
    // The token list starts with a valid expression, but there remains 
    // something. If the remainder consists out of whitespace only, the 
    // expression is still valid.
    iterator_type next = hit.stop;
    
        while (next != last) {
            switch (static_cast<unsigned int>(token_id(*next))) {
            case T_SPACE:
            case T_SPACE2:
            case T_CCOMMENT:
                break;                      // ok continue
                
            case T_NEWLINE:
            case T_EOF:
            case T_CPPCOMMENT:              // contains newline
                return bool(result);        // expression is valid
                
            default:
            // expression is illformed
                if (if_block_status) {
                    string_type expression = as_string<string_type>(first, last);
                    if (0 == expression.size()) 
                        expression = "empty expression";
                    BOOST_WAVE_THROW(preprocess_exception, ill_formed_expression, 
                        expression.c_str(), act_pos);
                }
                else {
                //  as the if_block_status is false no errors will be reported
                    return false;
                }
            }
            ++next;
        }
    }

    if (closure_value::error_noerror != result.is_valid()) {
    // division by zero occured
        string_type expression = as_string<string_type>(first, last);
        if (0 == expression.size()) 
            expression = "empty expression";
            
        if (closure_value::error_division_by_zero == result.is_valid()) {
            BOOST_WAVE_THROW(preprocess_exception, division_by_zero, 
                expression.c_str(), act_pos);
        }
        else if (closure_value::error_overflow == result.is_valid()) {
            BOOST_WAVE_THROW(preprocess_exception, integer_overflow,
                expression.c_str(), act_pos);
        }
    }
    
// token sequence is a valid expression
    return bool(result);
}

#undef BOOST_WAVE_EXPRGRAMMAR_GEN_INLINE

///////////////////////////////////////////////////////////////////////////////
}   // namespace grammars
}   // namespace wave
}   // namespace boost

// the suffix header occurs after all of the code
#ifdef BOOST_HAS_ABI_HEADERS
#include BOOST_ABI_SUFFIX
#endif

#endif // !defined(CPP_EXPRESSION_GRAMMAR_HPP_099CD1A4_A6C0_44BE_8F24_0B00F5BE5674_INCLUDED)
