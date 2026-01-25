#ifndef STATEMENT_HXX
#define STATEMENT_HXX

#include <tao/pegtl.hpp>
#include "Whitespace.hxx"
#include "Operator.hxx"
#include "Identifier.hxx"
#include "Int.hxx"

using tao::pegtl::star;
using tao::pegtl::seq;
using tao::pegtl::if_must;
using tao::pegtl::until;
using tao::pegtl::opt;
using tao::pegtl::sor;
using tao::pegtl::eof;
using tao::pegtl::one;

using wheel::ast::grammar::Whitespace;
using wheel::ast::grammar::AssignOperator;
using wheel::ast::grammar::Identifier;
using wheel::ast::grammar::IntKeyword;

namespace wheel::ast::grammar {
    template <typename Rule>
    using ignore_whitespace = seq<Rule, star<Whitespace>>;

    struct DeclarationStatement : seq
        <
            ignore_whitespace<IntKeyword>,
            ignore_whitespace<Identifier>,
            ignore_whitespace<AssignOperator>,
            ignore_whitespace<Int>
        >
    {};

    struct Statement : seq 
        <   
            star<Whitespace>,
            ignore_whitespace<DeclarationStatement>,
            sor
            <
                ignore_whitespace<one<';'>>,
                seq<one<'\n'>, star<Whitespace>>, 
                eof
            >
        >
    {};

    struct Program : until<eof, Statement> {};
}

#endif // STATEMENT_HXX