#ifndef INT_TYPE_HXX
#define INT_TYPE_HXX
#include <cstdint>
#include <ast/include/node_ast.hxx>

using wheel::ast::DataType;

namespace wheel::ast::types {
    class IntType {
        public:
            using ValueType = int32_t;

        private:
            ValueType m_value_type;

        public:
            explicit IntType(ValueType value);
            ValueType value() const noexcept;
            void set_value(ValueType value_type) noexcept;
            static constexpr DataType type_id() {
                return DataType::Int;
            }

            IntType &operator = (const IntType &other) noexcept {
                if(this != &other) {
                    this->m_value_type = other.m_value_type;
                }

                return *this;
            }

            explicit operator bool() const noexcept {
                return m_value_type != 0;
            }

            IntType &operator = (ValueType raw_value) noexcept {
                this->m_value_type = raw_value;
                return *this;
            }

    };
}

#endif // INT_TYPE_HXX