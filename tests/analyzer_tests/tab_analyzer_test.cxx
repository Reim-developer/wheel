#include <analyzer/tab.hxx>
#include <__internal/__assert.hxx>
#include <format>
#include <iostream>

using analyzer::TabAnalyzer;
using analyzer::IndentError;
using analyzer::IndentType;
using std::boolalpha;
using std::format;
using std::cout;

void test_leading_space_error() {
    cout << "'test_leading_space_error'\n";

    const char *source = " ";

    auto result = TabAnalyzer::analyzer(source, source + 1, 1);
    auto has_success = result.errors.empty();
    internal_assert(!has_success, format(
        "Expected '<result.errors.empty()>': == {}, but got {}",
        has_success, true
    ));
    
    auto error_type          = result.errors[0].error_type;
    auto expected_error_type = IndentError::LEADING_WHITE_SPACE_ERROR;

    internal_assert(
        error_type == expected_error_type,
        format(
            "Expected error type {}, but got {}",
            static_cast<int>(expected_error_type), 
            static_cast<int>(error_type))
    );

    cout << boolalpha;
    cout << "'<has_error_empty>:' " << static_cast<bool>(has_success) << "\n";
    cout << "'<error_message>: '" << result.errors[0].messsage << "\n";
    cout << "'<error_type>: '" << error_type << "\n";
}

void test_leading_tab_error() {
    cout << " 'test_leading_tab_error'\n";

    const char *source = "\t";

    auto result = TabAnalyzer::analyzer(source, source + 1, 1);
    auto has_success = result.errors.empty();

    internal_assert(!has_success, format(
        "Expected '<result.errors.empty()>': == {}, but got {}",
        has_success, true
    ));
    
    auto error_type          = result.errors[0].error_type;
    auto expected_error_type = IndentError::LEADING_TAB_ERROR;

    internal_assert(
        error_type == expected_error_type,
        format(
            "Expected error type {}, but got {}",
            static_cast<int>(expected_error_type), 
            static_cast<int>(error_type))
    );

    cout << boolalpha;
    cout << "'<has_error_empty>:' " << static_cast<bool>(has_success) << "\n";
    cout << "'<error_message>: '" << result.errors[0].messsage << "\n";
    cout << "'<error_type>: '" << error_type << "\n";
}

void test_mixed_indent_error() {
    cout << "'test_mixed_indent_error'\n";
    const char *source = " \t";

    auto result = TabAnalyzer::analyzer(source, source + 2, 1);
    auto has_success = result.errors.empty();
    auto errors = result.errors;

     internal_assert(
        errors[0].error_type == IndentError::LEADING_WHITE_SPACE_ERROR,
        format(
            "Expected {}, but got {} for <IndentError>",
            static_cast<int>(IndentError::LEADING_WHITE_SPACE_ERROR),
            static_cast<int>(errors[0].error_type)
        )
    );

    internal_assert(
        errors[1].error_type == IndentError::MIXED_INDENTATION_ERROR,
        format(
            "Expected {}, but got {} for <IndentError>",
            static_cast<int>(IndentError::MIXED_INDENTATION_ERROR),
            static_cast<int>(errors[1].error_type)
        )
    );

    for (auto error : errors) {
        cout << boolalpha;
        cout << "<has_error_empty>: " << has_success << "\n";
        cout << "<error_message>: " << error.messsage << "\n";
    }
}

void test_newline_success_ok() {
    cout << "'test_newline_success_ok'\n";

    const char *source = "\n";
    auto result = TabAnalyzer::analyzer(source, source + 1, 1);
    auto has_error_empty = result.errors.empty();
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
    cout << boolalpha;
    cout << "<has_error_empty>: " << has_error_empty << "\n";
    cout << "<has_indents_empty>: " << has_indents_empty << "\n";
    cout << "<indent_type>: " << static_cast<int>(indent_type) << "\n";
}

void test_no_leading_whitespace_success_ok() {
    cout << "'test_no_leading_whitespace_success_ok'\n";

    const char *source = "pseudo_code...";
    auto result = TabAnalyzer::analyzer(source, source + 0, 1);
    auto has_empty_errors   = result.errors.empty();
    auto has_empty_indents  = result.indents.empty();

    internal_assert(has_empty_errors, format(
        "Expected {}, but got {} for <has_empty_errors>",
        true, has_empty_errors
    ));

    internal_assert(has_empty_indents, format(
        "Expected {}, but gor {} for <has_empty_indents>",
        true, has_empty_indents
    ));

    cout << boolalpha;
    cout << "<has_empty_errors>: " << has_empty_errors << "\n";
    cout << "<has_empty_indents>: " << has_empty_indents << "\n";
}

int main() {
    test_leading_space_error();
    test_leading_tab_error();
    test_mixed_indent_error();
    test_newline_success_ok();
    test_no_leading_whitespace_success_ok();
    return 0;
}