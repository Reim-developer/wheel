#ifndef NODE_AST_HXX
#define NODE_AST_HXX

namespace wheel::ast {
    enum class DataType {
        Void,
        Int,
        String
    };

    struct Node {
        virtual ~Node() = default;
        virtual DataType get_type() const = 0;
    };
}

#endif // NODE_AST_HXX