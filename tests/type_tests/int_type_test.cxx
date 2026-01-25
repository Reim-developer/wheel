#include <ast/types/include/int_type.hxx>
#include <iostream>
#include <__internal/__assert.hxx>
#include <format>
#include <climits>

using std::cout;
using std::format;
using wheel::ast::types::IntType;

void test_positive_numbers() {
    cout << "[TEST] 'test_positive_numbers'\n";

    const auto variable_number = 100;
    IntType number(variable_number);

    const auto actual_number = number.value();
    const auto error_message = format("expected value {}, but found {} ",variable_number, actual_number);
    internal_assert(number.value() == variable_number, error_message.c_str());

    cout << "<variable_number>: " << variable_number << "\n";
    cout << "<number.value()>: " << actual_number << "\n";
}

void test_negative_numbers() {
    cout << "[TEST] 'test_negative_numbers'\n";

    const auto negative_number = -100;
    IntType number(-100);

    const auto actual_negative_number = number.value();
    const auto error_message = format(
        "expected negative number {}, but found {} ", 
        negative_number, actual_negative_number
    );
    internal_assert(number.value() == -100, error_message.c_str());

    cout << "<expected_negative_number>: " << negative_number << "\n";
    cout << "<actual_negative_number>: " << actual_negative_number << "\n";
}

void test_zero_value() {
    cout << "[TEST]" << "'test_zero_value'\n";
    const auto zero_number = 0;
    IntType number(zero_number);
    const auto actual = number.value();

    internal_assert(actual == 0, format("Expected {}, but found {}", zero_number, actual));
    internal_assert(static_cast<bool>(actual) == false, format("actual {} should be false", actual));

    cout << "<zero_number>: " << zero_number << "\n";
    cout << "<actual>: " << actual << "\n";
}

void test_int_boundaries() {
    cout << "[TEST]" << "'test_int_boundaries'\n";

    IntType max_int_type(INT_MAX);

    internal_assert(
        max_int_type.value() == INT_MAX, 
        format("Expected 'INT_MAX' {}, but found {}", 
            INT_MAX, max_int_type.value())
    );

    IntType min_int_type(INT_MIN);

    internal_assert(
        min_int_type.value() == INT_MIN,
        format("Expected 'INT_MIN' {}, but found {}", 
            INT_MIN, min_int_type.value())
    );

    cout << "<max_int_type>: " << max_int_type.value() << "\n";
    cout << "<min_int_type>: " << min_int_type.value() << "\n";
}

void test_assignment_operator() {
    cout << "[TEST]" << "'test_assignment_operator'\n";
    
    const auto integer_a = 460;
    const auto integer_b = 270;

    IntType number_a(integer_a);
    IntType number_b(integer_b);

    internal_assert(number_a.value() == integer_a, format(
        "Intial 'number_a' mismatch, expect {}, but found {}",
        integer_a, number_a.value()
    ));

    internal_assert(number_b.value() == integer_b, format(
        "Intial 'number_b' mismatch, expect {}, but found {}",
        integer_b, number_b.value()
    ));

    cout << "[Before Assignment] <number_a.value()>: " << number_a.value() << "\n";
    cout << "[Before Assignment] <number_b.value()>: " << number_b.value() << "\n";

    number_b = number_a;

    internal_assert(number_b.value() == number_a.value(), format(
        "'number_b' should be {}, but found {}",
        number_a.value(), number_b.value()
    ));
    internal_assert(number_a.value() == integer_a, format(
        "'number_a' was modified during assignment, expected {}, but found {}",
        integer_a, number_a.value()
    ));

    cout << "[After Assigment] <number_a.value()>: " << number_a.value() << "\n";
    cout << "[After Assigment] <number_b.value()>: " << number_b.value() << "\n";
}

void test_self_assigment() {
    cout << "[TEST]" << "'test_self_assigment'\n";

    const auto variable_a = 491;
    IntType number_a(variable_a);

    IntType &ref_to_a = number_a;
    ref_to_a = ref_to_a;

    internal_assert(number_a.value() == variable_a, format(
        "Self-assigment corrupted value, expected {}, but found {}",
        variable_a, number_a.value()
    ));

    cout << "[After Self-Assigment] <number_a.value()>: " << number_a.value() << "\n";
}

void test_raw_assigment() {
    cout << "[TEST]" << "'test_raw_assigment'\n";

    const auto variable     = 100;
    const auto variable_raw = 200;

    IntType number(variable);

    cout << "[Before Raw Assigment] <variable()>: " << variable << "\n";
    cout << "[Before Raw Assigment] <variable_raw()>: " << variable_raw << "\n";
    cout << "[Before Raw Assigment] <number_value()>: " << number.value() << "\n";

    number = variable_raw;

    cout << "[After Raw Assigment] <variable()>: " << variable << "\n";
    cout << "[After Raw Assigment] <variable_raw()>: " << variable_raw << "\n";
    cout << "[After Raw Assigment] <number_value()>: " << number.value() << "\n";

    internal_assert(
        number.value() != variable, format(
            "Raw assigment failed, expected {}, but found {}",
            variable_raw, number.value())
    );

    internal_assert(number.value() == variable_raw, format(
        "Raw assigment failed, expected {}, but found {}",
        variable_raw, number.value())
    );
}

int main() {
    test_positive_numbers();
    test_negative_numbers();
    test_zero_value();
    test_int_boundaries();
    test_assignment_operator();
    test_self_assigment();
    test_raw_assigment();
}