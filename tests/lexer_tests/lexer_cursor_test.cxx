#include "__lexer_test_helper/__helper.hxx"
#include "__internal/__assert.hxx"
#include <wheel_lexer/cursor.hxx>
#include "format"

using wheel_lexer::Cursor;
using std::format;
using std::string;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

TEST(test_cursor_first_character)
    const char* input = "something input";

    Cursor cursor(input);

    const auto result = cursor.first();
    internal_assert(result == 's', format(
        "Expected: 's', but got {}",
        result
    ));

    DEBUG_OUTPUT(format("Expected Cursor First: 's'. Got: '{}'", result));
DONE

TEST(test_cursor_empty_input)
    Cursor cursor("");
    
    internal_assert(cursor.first() == '\0', format(
        "Expected '0', but got '{}'",
        cursor.first()
    ));
DONE 

TEST(test_cursor_advance_and_eof)
    const char *input = "abc";

    Cursor cursor(input);
    const char first = cursor.first();
    internal_assert(cursor.position() == 0, format(
        "Expected Cursor Position: 0. But got: {}",
        cursor.position()
    ));
    internal_assert(first == 'a', format(
        "Expected Cursor First: 'a'. But got: {}",
        first
    ));

    cursor.advance();
    const char first2 = cursor.first();
    internal_assert(cursor.position() == 1, format(
        "Expected Cursor Postion: 1. But got: {}",
        cursor.position()
    ));
    internal_assert(first2 == 'b', format(
        "Expected Cursor First: 'b'. But got: {}",
        first2
    ));

    cursor.advance(999);
    internal_assert(cursor.position() == 3, format(
        "Expected Cursor Position: 3. But got: {}",
        cursor.position()
    ));
    internal_assert(cursor.first() == '\0', format(
        "Expected Cursor First: ''. But got: {}",
        cursor.first()
    ));
DONE 

TEST(test_cursor_fuzzing_operations)
    auto random_input = random_str(1000);

    Cursor cursor(random_input);
    size_t total_advances = 0;

    random_device               random;
    mt19937                     generator(random());
    uniform_int_distribution<>  distribution(0, 1);

    for (int index = 0; index < 2000; ++index) {
        if(cursor.is_eof()) break;
        if(distribution(generator) == 0) {
            char character = cursor.bump();
            static_cast<void>(character);

        } else {
            size_t  steps = distribution(generator) + 1;
            cursor.advance(steps);
        }
    }

    internal_assert(cursor.position() == cursor.source_view.size(), format(
        "Expected Cursor Postion: {}, but got {}",
        cursor.position(), cursor.source_view.size()
    ));
DONE 

TEST(test_cursor_bump_and_previous)
    const char *input = "xyz";

    Cursor cursor(input);
    internal_assert(!cursor.has_previous(), "Should not have previous at start");
    internal_assert(cursor.previous() == '\0', "Previous char should be EOF");

    char c1 = cursor.bump();
    internal_assert(c1 == 'x', "First bump should return 'x'");
    internal_assert(cursor.previous() == 'x', "Previous char should be 'x'");
    internal_assert(cursor.has_previous() == true, "Should have previuos now");

    char c2 = cursor.bump();
    internal_assert(c2 == 'y', "Second bump should return 'y'");
    internal_assert(cursor.previous() == 'y', "Previous char should be 'y'");
    internal_assert(cursor.position() == 2, "Position should be '2' after 2 bumps");
DONE 

TEST_ENTRY
    RUN_TEST(test_cursor_first_character)
    RUN_TEST(test_cursor_empty_input)
    RUN_TEST(test_cursor_advance_and_eof)
    RUN_TEST(test_cursor_fuzzing_operations)
    RUN_TEST(test_cursor_bump_and_previous)
ENTRY_END