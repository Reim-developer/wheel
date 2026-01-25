#ifndef OPERATOR_HXX
#define OPERATOR_HXX

#include <tao/pegtl.hpp>

using tao::pegtl::one;

namespace wheel::ast::grammar {
    struct AssignOperator : one<'='> {};
}

#endif // OPERATOR_HXX