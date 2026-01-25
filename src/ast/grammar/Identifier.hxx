#ifndef IDENTIFIER_HXX
#define IDENTIFIER_HXX
#include <tao/pegtl.hpp>

using tao::pegtl::identifier;

namespace wheel::ast::grammar {
    struct Identifier : identifier {};
}

#endif // IDENTIFIER_HXX