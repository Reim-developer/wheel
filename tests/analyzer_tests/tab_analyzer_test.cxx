#include <analyzer/tab.hxx>
#include <analyzer/error.hxx>
#include <__internal/__assert.hxx>
#include <format>
#include <iostream>

using analyzer::TabAnalyzer;
using analyzer::DiagnosticCategory;
using analyzer::DiagnosticBuilder;
using analyzer::IndentType;
using std::boolalpha;
using std::format;
using std::cout;

void test_leading_space_error() {
    DEBUG_OUTPUT("'test_leading_space_error'");

    const char *source     = " ";
    const char *file_name = "test.wheel";

    auto result = TabAnalyzer::analyzer(source, source + 1, 1, source, file_name);
    auto has_success = result.diagnostics.empty();
    internal_assert(!has_success, format(
        "Expected '<result.diagnostics.empty()>': == {}, but got {}",
        has_success, true
    ));
    
    auto error_type          = result.diagnostics[0].category;
    auto expected_error_type = DiagnosticCategory::INDENTATION;

    internal_assert(
        result.diagnostics.at(0).level == Diagnostic::Level::ERROR,
        format(
            "Expected {}, but got {} for <Diagnostic::Level::ERROR>",
            static_cast<int>(result.diagnostics.at(0).level),
            static_cast<int>(Diagnostic::Level::ERROR)
        )
    );

    internal_assert(
        error_type == expected_error_type,
        format(
            "Expected error type {}, but got {}",
            static_cast<int>(expected_error_type), 
            static_cast<int>(error_type))
    );
    DEBUG_OUTPUT(format(
        "<has_error_empty>: {}", has_success
    ));
    DEBUG_OUTPUT(format(
        "<error_message>: {}", result.diagnostics[0].full_message
    ));
    DEBUG_OUTPUT(format(
        "<error_type>: {}", static_cast<int>(error_type)
    ));
    DEBUG_OUTPUT(format(
        "<quick_fix_message>: {}", result.diagnostics[0].quick_fix
    ));
}

void test_leading_tab_error() {
    DEBUG_OUTPUT("'test_leading_tab_error'");

    const char *source      = "\t";
    const char *file_name   = "test.wheel";

    auto result = TabAnalyzer::analyzer(source, source + 1, 1, source, file_name);
    auto has_success = result.diagnostics.empty();

    internal_assert(!has_success, format(
        "Expected '<result.diagnostics.empty()>': == {}, but got {}",
        has_success, true
    ));
    
    auto error_type          = result.diagnostics[0].category;
    auto expected_error_type = DiagnosticCategory::INDENTATION;

    internal_assert(
        result.diagnostics.at(0).level == Diagnostic::Level::ERROR,
        format(
            "Expected {}, but got {} for <Diagnostic::Level::ERROR>",
            static_cast<int>(result.diagnostics.at(0).level),
            static_cast<int>(Diagnostic::Level::ERROR)
        )
    );

    internal_assert(
        error_type == expected_error_type,
        format(
            "Expected error type {}, but got {}",
            static_cast<int>(expected_error_type), 
            static_cast<int>(error_type))
    );

    DEBUG_OUTPUT(format(
        "<error_message>: {}", result.diagnostics[0].full_message
    ));
    DEBUG_OUTPUT(format(
        "<error_type>: {}", static_cast<int>(error_type)
    ));
    DEBUG_OUTPUT(format(
        "<has_error_empty>: ", has_success
    ));
    DEBUG_OUTPUT(format(
        "<quick_fix_messsage>: {}", result.diagnostics[0].quick_fix
    ));
}

void test_mixed_indent_error() {
    DEBUG_OUTPUT("'test_mixed_indent_error'");
    const char *source      = "  \t\t";
    const char *file_name   = "test.wheel";

    auto result = TabAnalyzer::analyzer(source, source + 4, 1, source, file_name);
    auto has_success = result.diagnostics.empty();
    auto errors = result.diagnostics;

    internal_assert(
        errors.at(0).level == Diagnostic::Level::ERROR,
        format(
            "Expected {}, but got {} for <Diagnostic::Level::ERROR>",
            static_cast<int>(errors.at(0).level),
            static_cast<int>(Diagnostic::Level::ERROR)
        )
    );
    internal_assert(
        errors.at(0).category == DiagnosticCategory::INDENTATION,
        format(
            "Expected {}, but got {} for <DiagnosticCategory>",
            static_cast<int>(DiagnosticCategory::INDENTATION),
            static_cast<int>(errors[0].category)
        )
    );

    DEBUG_OUTPUT(format(
        "<has_error_empty>: {}", has_success
    ));
    DEBUG_OUTPUT(format(
        "<error_message>: {}", errors.at(0).full_message
    ));
    DEBUG_OUTPUT(format(
        "<quick_fix_message>: {}", errors.at(0).quick_fix
    ));
}

void test_newline_success_ok() {
    DEBUG_OUTPUT("'test_newline_success_ok'");

    const char *source      = "\n";
    const char *file_name   = "test.wheel";

    auto result = TabAnalyzer::analyzer(source, source + 1, 1, source, file_name);
    auto has_error_empty = result.diagnostics.empty();
    auto has_indents_empty = result.indents.empty();

    internal_assert(!has_indents_empty, format(
        "Expected {}, but got {} for <has_indent_empty>",
        true, has_indents_empty
    ));
    auto indent_type = result.indents[0];

    internal_assert(
        indent_type == IndentType::NEWLINE,
        format(
        "Expected {}, but got {} for <indent_type>",
        static_cast<int>(IndentType::NEWLINE), 
        static_cast<int>(indent_type)
    ));

    DEBUG_OUTPUT(format(
        "<has_error_empty>: {}", has_error_empty
    ));
    DEBUG_OUTPUT(format(
        "<has_indents_empty>: {}", has_indents_empty
    ));
    DEBUG_OUTPUT(format(
        "<indent_type>: {}", static_cast<int>(indent_type)
    ));
}

void test_no_leading_whitespace_success_ok() {
    DEBUG_OUTPUT("'test_no_leading_whitespace_success_ok'");

    const char *source      = "pseudo_code...";
    const char *file_name   = "test.wheel";

    auto result = TabAnalyzer::analyzer(source, source + 0, 1, file_name, source);
    auto has_empty_errors   = result.diagnostics.empty();
    auto has_empty_indents  = result.indents.empty();

    internal_assert(has_empty_errors, format(
        "Expected {}, but got {} for <has_empty_errors>",
        true, has_empty_errors
    ));

    internal_assert(has_empty_indents, format(
        "Expected {}, but gor {} for <has_empty_indents>",
        true, has_empty_indents
    ));

    DEBUG_OUTPUT(format(
        "<has_empty_errors>: {}", has_empty_errors
    ));
    DEBUG_OUTPUT(format(
        "<has_empty_indents>: {}", has_empty_indents
    ));
}

int main() {
    test_leading_space_error();
    test_leading_tab_error();
    test_mixed_indent_error();
    test_newline_success_ok();
    test_no_leading_whitespace_success_ok();
    return 0;
}