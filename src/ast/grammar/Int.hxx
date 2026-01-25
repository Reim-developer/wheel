#ifndef INT_HXX
#define INT_HXX
#include <tao/pegtl.hpp>

using tao::pegtl::if_must;
using tao::pegtl::one;
using tao::pegtl::digit;
using tao::pegtl::plus;
using tao::pegtl::opt;

namespace wheel::ast::grammar {
    using tao::pegtl::string;

    struct Int : if_must <
        opt<one<'+', '-'>>,
        plus<digit>>
    {};

    struct IntKeyword : string<'i', 'n', 't'> {};
}

#endif // INT_HXX