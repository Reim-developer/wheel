#include <ast/types/include/int_type.hxx>

using Self      = wheel::ast::types::IntType;
using ValueType = Self::ValueType;

Self::IntType(ValueType value_type) : m_value_type(value_type) {}

ValueType Self::value() const noexcept {
    return m_value_type;
}

void Self::set_value(ValueType value_type) noexcept {
    m_value_type = value_type;
}