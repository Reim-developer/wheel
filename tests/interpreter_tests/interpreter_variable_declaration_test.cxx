#include <__internal/__assert.hxx>

#include <array>
#include <initializer_list>
#include <iostream>
#include <sstream>

#include <wheel_interpreter/interpreter.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_semantic/program.hxx>

using wheel_interpreter::RuntimeErrorCode;
using wheel_interpreter::RuntimeValueKind;
using wheel_interpreter::WheelInterpreter;
using wheel_memory::Arena;
using wheel_parser::ast::SymbolID;
using wheel_semantic::DefineGlobalStatement;
using wheel_semantic::ExecutableStatement;
using wheel_semantic::Operand;
using wheel_semantic::OperandList;
using wheel_semantic::PrintlnStatement;
using wheel_semantic::ProgramView;
using wheel_semantic::Value;

namespace {
    [[nodiscard]] OperandList make_operand_list(
        Arena &arena,
        std::initializer_list<Operand> operands
    ) {
        #if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
            OperandList list(&arena);
        #else
            OperandList list;
            list.reserve(operands.size());
        #endif

        for (const Operand &operand : operands) {
            list.push_back(operand);
        }

        return list;
    }
} // namespace

TEST(test_interpreter_executes_define_global_statement)
    Arena arena;
    constexpr SymbolID symbol = 42;
    const auto *statement = arena.allocate<DefineGlobalStatement>(
        nullptr,
        symbol,
        Operand::from_constant(nullptr, Value::from_int(10))
    );
    const std::array<const ExecutableStatement *, 1> statements = {statement};

    WheelInterpreter interpreter(arena);
    const bool interpreted = interpreter.execute(ProgramView{
        statements.data(),
        statements.size()
    });

    assert_eq(interpreted, true);
    assert_eq(interpreter.error_count(), 0);
    assert_eq(interpreter.binding_count(), 1);

    const auto *value = interpreter.find_value(symbol);
    assert_eq(value == nullptr, false);
    assert_eq(static_cast<uint8_t>(value->kind), static_cast<uint8_t>(RuntimeValueKind::Int));
    assert_eq(value->int_value, 10);
DONE

TEST(test_interpreter_reports_duplicate_runtime_binding)
    Arena arena;
    constexpr SymbolID symbol = 7;
    const auto *first = arena.allocate<DefineGlobalStatement>(
        nullptr,
        symbol,
        Operand::from_constant(nullptr, Value::from_int(1))
    );
    const auto *second = arena.allocate<DefineGlobalStatement>(
        nullptr,
        symbol,
        Operand::from_constant(nullptr, Value::from_int(2))
    );
    const std::array<const ExecutableStatement *, 2> statements = {first, second};

    WheelInterpreter interpreter(arena);
    const bool interpreted = interpreter.execute(ProgramView{
        statements.data(),
        statements.size()
    });
    const auto *errors = interpreter.errors_data();

    assert_eq(interpreted, false);
    assert_eq(interpreter.error_count(), 1);
    assert_eq(static_cast<uint16_t>(errors[0].code), static_cast<uint16_t>(RuntimeErrorCode::DuplicateBinding));
DONE

TEST(test_interpreter_println_writes_multiple_arguments_to_stdout)
    Arena arena;
    constexpr SymbolID name_symbol = 99;
    constexpr SymbolID value_symbol = 100;

    const auto *define_name = arena.allocate<DefineGlobalStatement>(
        nullptr,
        name_symbol,
        Operand::from_constant(nullptr, Value::from_string("Wheel"))
    );
    const auto *define_value = arena.allocate<DefineGlobalStatement>(
        nullptr,
        value_symbol,
        Operand::from_constant(nullptr, Value::from_int(42))
    );
    const auto *println_statement = arena.allocate<PrintlnStatement>(
        nullptr,
        "Hello, {} {}",
        make_operand_list(arena, {
            Operand::from_binding(nullptr, name_symbol),
            Operand::from_binding(nullptr, value_symbol),
        })
    );
    const std::array<const ExecutableStatement *, 3> statements = {
        define_name,
        define_value,
        println_statement
    };

    std::ostringstream output_stream;
    auto *original_buffer = std::cout.rdbuf(output_stream.rdbuf());

    WheelInterpreter interpreter(arena);
    const bool interpreted = interpreter.execute(ProgramView{
        statements.data(),
        statements.size()
    });

    std::cout.rdbuf(original_buffer);

    assert_eq(interpreted, true);
    assert_eq(output_stream.str(), "Hello, Wheel 42\n");
DONE

TEST_MAIN
    RUN(test_interpreter_executes_define_global_statement)
    RUN(test_interpreter_reports_duplicate_runtime_binding)
    RUN(test_interpreter_println_writes_multiple_arguments_to_stdout)
END_MAIN
