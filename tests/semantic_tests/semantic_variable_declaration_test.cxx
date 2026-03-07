#include <__internal/__assert.hxx>

#include <wheel_lexer/lexer.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_parser/parser.hxx>
#include <wheel_semantic/semantic.hxx>
#include <wheel_semantic/errors.hxx>

using wheel_lexer::Lexer;
using wheel_memory::Arena;
using wheel_parser::WheelParser;
using wheel_parser::ast::StringInterner;
using wheel_semantic::DefineGlobalStatement;
using wheel_semantic::ExecutableStatementKind;
using wheel_semantic::OperandKind;
using wheel_semantic::SemanticAnalyzer;
using wheel_semantic::SemanticErrorCode;
using wheel_semantic::ValueKind;
using wheel_semantic::SemanticErrorCode;

TEST(test_semantic_resolves_int_variable_declaration)
    Arena arena;
    Lexer lexer("var x: int = 10");
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);
    parser.parse();

    assert_eq(parser.error_count(), 0);

    SemanticAnalyzer semantic(arena, interner);
    const bool analyzed = semantic.analyze(parser.statements_data(), parser.statement_count());

    assert_eq(analyzed, true);
    assert_eq(semantic.error_count(), 0);
    assert_eq(semantic.statement_count(), 1);

    const auto *statement = semantic.statements_data()[0];
    assert_eq(static_cast<uint8_t>(statement->kind), static_cast<uint8_t>(ExecutableStatementKind::DefineGlobal));

    const auto *define_global = static_cast<const DefineGlobalStatement *>(statement);
    assert_eq(define_global->name, interner.intern("x"));
    assert_eq(static_cast<uint8_t>(define_global->initializer.kind), static_cast<uint8_t>(OperandKind::Constant));
    assert_eq(static_cast<uint8_t>(define_global->initializer.constant.kind), static_cast<uint8_t>(ValueKind::Int));
    assert_eq(define_global->initializer.constant.int_value, 10);
DONE

TEST(test_semantic_reports_type_mismatch)
    Arena arena;
    Lexer lexer("var x: int = \"hello\"");
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);
    parser.parse();

    assert_eq(parser.error_count(), 0);

    SemanticAnalyzer semantic(arena, interner);
    const bool analyzed = semantic.analyze(parser.statements_data(), parser.statement_count());
    const auto *errors = semantic.errors_data();

    assert_eq(analyzed, false);
    assert_eq(semantic.error_count(), 1);
    assert_eq(static_cast<uint16_t>(errors[0].code), static_cast<uint16_t>(SemanticErrorCode::TYPE_MISMATCH));
DONE

TEST(test_semantic_reports_duplicate_symbol)
    Arena arena;
    Lexer lexer("var x: int = 1\nvar x: int = 2");
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);
    parser.parse();

    assert_eq(parser.error_count(), 0);
    assert_eq(parser.statement_count(), 2);

    SemanticAnalyzer semantic(arena, interner);
    const bool analyzed = semantic.analyze(parser.statements_data(), parser.statement_count());
    const auto *errors = semantic.errors_data();

    assert_eq(analyzed, false);
    assert_eq(semantic.error_count(), 1);
    assert_eq(static_cast<uint16_t>(errors[0].code), static_cast<uint16_t>(SemanticErrorCode::DUPLICATE_SYMBOL));
DONE

TEST_MAIN
    RUN(test_semantic_resolves_int_variable_declaration)
    RUN(test_semantic_reports_type_mismatch)
    RUN(test_semantic_reports_duplicate_symbol)
END_MAIN
