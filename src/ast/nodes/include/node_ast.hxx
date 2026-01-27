#ifndef NODE_AST_HXX
#define NODE_AST_HXX
#include "data_type.hxx"

using wheel::ast::DataType;

namespace wheel::ast {
    struct Node {
        protected:
            explicit Node(DataType data_type);

        public:
            virtual ~Node() = default;
            virtual DataType get_type() const noexcept;

        private:
            DataType m_type;
    };
}

#endif // NODE_AST_HXX