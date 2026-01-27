#include "include/node_ast.hxx"

using wheel::ast::Node;
using wheel::ast::DataType;

Node::Node(DataType data_type) : m_type(data_type) {}

DataType Node::get_type() const noexcept {
    return m_type;
}