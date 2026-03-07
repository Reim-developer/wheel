#include "wheel_interpreter/interpreter.hxx"

#include <iostream>
#include <sstream>

using wheel_interpreter::RuntimeBinding;
using wheel_interpreter::RuntimeError;
using wheel_interpreter::RuntimeErrorCode;
using wheel_interpreter::RuntimeValue;
using wheel_interpreter::WheelInterpreter;
using wheel_semantic::DefineGlobalStatement;
using wheel_semantic::Operand;
using wheel_semantic::OperandKind;
using wheel_semantic::ExecutableStatementKind;
using wheel_semantic::PrintlnStatement;
using wheel_semantic::ProgramView;

const char *runtime_error_message(RuntimeErrorCode code) noexcept {
        switch (code) {
            case RuntimeErrorCode::DuplicateBinding:
                return "duplicate runtime binding";
            case RuntimeErrorCode::UnsupportedStatement:
                return "unsupported executable statement";
            case RuntimeErrorCode::UnknownBinding:
                return "unknown runtime binding";
        }

        return "unknown runtime error";
    }

void append_runtime_value(std::ostringstream &stream, const RuntimeValue &value) {
    switch (value.kind) {
        case wheel_semantic::ValueKind::Int:
            stream << value.int_value;
            return;
        case wheel_semantic::ValueKind::String:
            stream << value.string_value;
            return;
    }
}


#if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
WheelInterpreter::WheelInterpreter(Arena &arena) noexcept :
    m_bindings(&arena), m_errors(&arena) {
}
#else
WheelInterpreter::WheelInterpreter(Arena &arena) noexcept :
    m_bindings(), m_errors() {
}
#endif

void WheelInterpreter::push_error(RuntimeErrorCode code, const Token *token) noexcept {
    m_errors.push_back(RuntimeError{
        code,
        token,
        runtime_error_message(code)
    });
}

const RuntimeBinding *WheelInterpreter::find_binding(SymbolID name) const noexcept {
    const RuntimeBinding *bindings = m_bindings.data();
    for (size_t index = 0; index < m_bindings.size(); ++index) {
        if (bindings[index].name == name) {
            return &bindings[index];
        }
    }

    return nullptr;
}

bool WheelInterpreter::resolve_operand(const Operand &operand, RuntimeValue &value) noexcept {
    switch (operand.kind) {
        case OperandKind::Constant:
            value = operand.constant;
            return true;
        case OperandKind::Binding: {
            const RuntimeValue *binding_value = find_value(operand.binding);
            if (binding_value == nullptr) {
                push_error(RuntimeErrorCode::UnknownBinding, operand.token);
                return false;
            }

            value = *binding_value;
            return true;
        }
    }

    push_error(RuntimeErrorCode::UnsupportedStatement, operand.token);
    return false;
}

bool WheelInterpreter::execute_define_global(
    const DefineGlobalStatement *statement
) noexcept {
    if (statement == nullptr) {
        push_error(RuntimeErrorCode::UnsupportedStatement, nullptr);
        return false;
    }

    if (find_binding(statement->name) != nullptr) {
        push_error(RuntimeErrorCode::DuplicateBinding, statement->token);
        return false;
    }

    RuntimeValue value = RuntimeValue::from_int(0);
    if (!resolve_operand(statement->initializer, value)) {
        return false;
    }

    m_bindings.push_back(RuntimeBinding{
        statement->name,
        value
    });

    return true;
}

bool WheelInterpreter::execute_println(const PrintlnStatement *statement) noexcept {
    if (statement == nullptr) {
        push_error(RuntimeErrorCode::UnsupportedStatement, nullptr);
        return false;
    }

    std::ostringstream rendered_line;
    const Operand *arguments = statement->arguments.data();
    const size_t argument_count = statement->arguments.size();
    size_t format_offset = 0;

    for (size_t index = 0; index < argument_count; ++index) {
        const size_t placeholder_index = statement->format_string.find("{}", format_offset);
        if (placeholder_index == std::string_view::npos) {
            push_error(RuntimeErrorCode::UnsupportedStatement, statement->token);
            return false;
        }

        rendered_line << statement->format_string.substr(
            format_offset,
            placeholder_index - format_offset
        );

        RuntimeValue value = RuntimeValue::from_int(0);
        if (!resolve_operand(arguments[index], value)) {
            return false;
        }

        append_runtime_value(rendered_line, value);
        format_offset = placeholder_index + 2;
    }

    rendered_line << statement->format_string.substr(format_offset);
    std::cout << rendered_line.str() << '\n';
    return true;
}

bool WheelInterpreter::execute_statement(const ExecutableStatement *statement) noexcept {
    if (statement == nullptr) {
        push_error(RuntimeErrorCode::UnsupportedStatement, nullptr);
        return false;
    }

    switch (statement->kind) {
        case ExecutableStatementKind::DefineGlobal:
            return execute_define_global(
                static_cast<const DefineGlobalStatement *>(statement)
            );
        case ExecutableStatementKind::Println:
            return execute_println(
                static_cast<const PrintlnStatement *>(statement)
            );
    }

    push_error(RuntimeErrorCode::UnsupportedStatement, statement->token);
    return false;
}

bool WheelInterpreter::execute(ProgramView program) noexcept {
    return execute(program.statements, program.count);
}

bool WheelInterpreter::execute(const ExecutableStatement *const *statements, size_t count) noexcept {
    reset();
    if (statements == nullptr && count > 0) {
        push_error(RuntimeErrorCode::UnsupportedStatement, nullptr);
        return false;
    }
    for (size_t index = 0; index < count; ++index) {
        execute_statement(statements[index]);
    }

    return m_errors.size() == 0;
}

void WheelInterpreter::reset() noexcept {
    m_bindings.clear();
    m_errors.clear();
}

size_t WheelInterpreter::binding_count() const noexcept {
    return m_bindings.size();
}

const RuntimeBinding *WheelInterpreter::bindings_data() const noexcept {
    return m_bindings.data();
}

const RuntimeValue *WheelInterpreter::find_value(SymbolID name) const noexcept {
    const RuntimeBinding *binding = find_binding(name);
    if (binding == nullptr) {
        return nullptr;
    }

    return &binding->value;
}

size_t WheelInterpreter::error_count() const noexcept {
    return m_errors.size();
}

const RuntimeError *WheelInterpreter::errors_data() const noexcept {
    return m_errors.data();
}

