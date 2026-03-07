#include <__internal/__assert.hxx>

#include <wheel_lexer/lexer.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_parser/parser.hxx>
#include <wheel_semantic/program.hxx>
#include <wheel_semantic/semantic.hxx>

using wheel_lexer::Lexer;
using wheel_memory::Arena;
using wheel_parser::WheelParser;
using wheel_parser::ast::StringInterner;
using wheel_semantic::ExecutableStatementKind;
using wheel_semantic::OperandKind;
using wheel_semantic::PrintlnStatement;
using wheel_semantic::SemanticAnalyzer;
using wheel_semantic::SemanticErrorCode;

TEST(test_semantic_lowers_println_with_multiple_binding_arguments)
    Arena arena;
    Lexer lexer(
        "var first: string = \"Wheel\"\n"
        "var answer: int = 42\n"
        "println(\"{} {}\". first. answer)"
    );
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);
    parser.parse();

    assert_eq(parser.error_count(), 0);
    assert_eq(parser.statement_count(), 3);

    SemanticAnalyzer semantic(arena, interner);
    const bool analyzed = semantic.analyze(parser.statements_data(), parser.statement_count());

    assert_eq(analyzed, true);
    assert_eq(semantic.error_count(), 0);
    assert_eq(semantic.statement_count(), 3);

    const auto *statement = semantic.statements_data()[2];
    assert_eq(static_cast<uint8_t>(statement->kind), static_cast<uint8_t>(ExecutableStatementKind::Println));

    const auto *println_statement = static_cast<const PrintlnStatement *>(statement);
    assert_eq(println_statement->format_string, "{} {}");
    assert_eq(println_statement->arguments.size(), 2);

    const auto *arguments = println_statement->arguments.data();
    assert_eq(static_cast<uint8_t>(arguments[0].kind), static_cast<uint8_t>(OperandKind::Binding));
    assert_eq(arguments[0].binding, interner.intern("first"));
    assert_eq(static_cast<uint8_t>(arguments[1].kind), static_cast<uint8_t>(OperandKind::Binding));
    assert_eq(arguments[1].binding, interner.intern("answer"));
DONE

TEST(test_semantic_reports_println_format_mismatch)
    Arena arena;
    Lexer lexer("println(\"{} {}\". 1)");
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);
    parser.parse();

    assert_eq(parser.error_count(), 0);

    SemanticAnalyzer semantic(arena, interner);
    const bool analyzed = semantic.analyze(parser.statements_data(), parser.statement_count());
    const auto *errors = semantic.errors_data();

    assert_eq(analyzed, false);
    assert_eq(semantic.error_count(), 1);
    assert_eq(static_cast<uint16_t>(errors[0].code), static_cast<uint16_t>(SemanticErrorCode::FormatArgumentMismatch));
DONE

TEST_MAIN
    RUN(test_semantic_lowers_println_with_multiple_binding_arguments)
    RUN(test_semantic_reports_println_format_mismatch)
END_MAIN
