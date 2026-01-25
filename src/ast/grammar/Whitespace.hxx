#ifndef WHITESPACE_HXX
#define WHITESPACE_HXX

#include <tao/pegtl.hpp>

using tao::pegtl::one;
using tao::pegtl::if_must;

namespace wheel::ast::grammar {
    struct Space : one <' '> {};

    struct Whitespace : one 
        <' ', '\t', '\n', '\r'>
    {};
}

#endif // WHITESPACE_HXX