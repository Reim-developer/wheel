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
using tao::pegtl::not_at;

using wheel::ast::grammar::AssignOperator;
using wheel::ast::grammar::Identifier;
using wheel::ast::grammar::IntKeyword;
using wheel::ast::grammar::Space;

namespace wheel::ast::grammar {
    template <typename Rule>
    using ignore_space = seq<Rule, star<Space>>;

    struct DeclarationStatement : seq
        <
            ignore_space<IntKeyword>,
            ignore_space<Identifier>,
            ignore_space<AssignOperator>,
            ignore_space<Int>
        >
    {};

    struct Statement : seq 
        <   
            if_must< not_at<Space> >,
            ignore_space<DeclarationStatement>,
            sor
            <
                ignore_space<one<';'>>,
                seq<one<'\n'>, star<Space>>, 
                eof
            >
        >
    {};

    struct Program : until<eof, Statement> {};
}

#endif // STATEMENT_HXX