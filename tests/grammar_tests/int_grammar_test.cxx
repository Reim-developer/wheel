#if defined(_MSC_VER)
    #define PEGTL_DISABLE_CONSTEVAL
#endif

#include <iostream>
#include <string>
#include <format>
#include <tao/pegtl.hpp>
#include <ast/grammar/Statement.hxx>
#include <__internal/__assert.hxx>

using std::string;
using std::format;
using std::cout;
using tao::pegtl::string_input;
using tao::pegtl::parse;
using tao::pegtl::parse_error;
using wheel::ast::grammar::Program;

void run_grammar_test(
        const string &name, const string &input, 
        bool expected_success) {
    string_input<> str_input(input, name);

    try {
        bool parse_result = parse<Program>(str_input);
        string message = format(
            "Test case '{}', expected {}, got {}",
            name, 
            (expected_success ? "success" : "failure"),
            (parse_result ? "success" : "failure")
        );

        internal_assert(parse_result == expected_success, message);
        cout << "[TEST] " << name << format(" with input <{}>", input) << "\n";

    } catch (const parse_error &error) {
        if(expected_success) {
            internal_assert(false, format("Test case {} failed, error: {}", name, error.what()));
        }
    }
}

void test_normal_variable_declaration() {
    run_grammar_test("'test_normal_variable_declaration'", "int x = 10", true);
}

void test_normal_variable_with_tab_start_failed() {
    run_grammar_test("' test_normal_variable_with_tab_start_failed'", "    int x = 10", false);
}

void test_with_semicolon_end_file() {
    run_grammar_test("'test_with_semicolon_end_file'", "int x = 10;", true);
}

void test_with_new_line() {
    run_grammar_test("'test_with_new_line'", "int x = 10\n", true);
}

void test_multiline_statements() {
    const auto statements = "int x = 10; int y = 20; int z = 30";
    run_grammar_test("'test_multiline_statements'", statements, true);
}

void test_multiline_statements_with_whitespace_start_failed() {
    const auto statements = " int x = 10; int y = 20; int z = 30";
    run_grammar_test("'test_multiline_statements_with_whitespace_start_failed'", statements, false);
}

void test_multiline_statements_with_whitespace_end_success() {
    const auto statements = "int x = 10; int y = 20; int z = 30 ";
    run_grammar_test("'test_multiline_statements_with_whitespace_end_success'", statements, true);
}

void test_multiline_statements_with_whitespace_end_start_failed() {
    const auto statements = " int x = 10; int y = 20; int z = 30 ";
    run_grammar_test("'test_multiline_statements_with_whitespace_end_start_failed'", statements, false);
}

void test_multiline_statements_with_tab_end_start_failed() {
    const auto statements = "   int x = 10; int y = 20; int z = 30  ";
    run_grammar_test("'test_multiline_statements_with_tab_end_start_failed'", statements, false);
}

void test_multiline_statements_without_semicolon_failed() {
    const auto statements = "int x = 10 int y = 20";
    run_grammar_test("'test_multiline_statements_without_semicolon_failed'", statements, false);
}

void test_multiline_statements_with_newline_indent_failed() {
    const auto statements = R"(
int x = 10
    int y = 20; int z = 30
    )";

    run_grammar_test("'test_multiline_statements_with_newline_indent_failed'", statements, false);
}

int main() {
    test_normal_variable_declaration();
    test_normal_variable_with_tab_start_failed();
    test_with_semicolon_end_file();
    test_with_new_line();
    test_multiline_statements();
    test_multiline_statements_with_whitespace_start_failed();
    test_multiline_statements_with_whitespace_end_success();
    test_multiline_statements_with_whitespace_end_start_failed();
    test_multiline_statements_with_tab_end_start_failed();
    test_multiline_statements_without_semicolon_failed();
    test_multiline_statements_with_newline_indent_failed();
    return 0;
}