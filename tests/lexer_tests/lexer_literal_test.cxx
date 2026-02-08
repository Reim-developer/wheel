#include "__lexer_test_helper/__helper.hxx"
#include "__internal/__assert.hxx"

#include <wheel_lexer/lexer.hxx>
#include <wheel_lexer/kind.hxx>

using wheel_lexer::Lexer;
using wheel_lexer::TokenKind;

TEST(test_int_literal)
    const char *source = "1923413";

    Lexer lexer(source);
    auto token = lexer.next_token();
    VERIFY_THIS(TokenKind::INT_LITERAL).token_as(token.kind);
    VERIFY_THIS(TokenKind::INT_LITERAL).token_str_eq_to(token, "1923413", STR_LEN(source));
    VERIFY_THIS(TokenKind::INT_LITERAL).token_metadata_eq_to(
        lexer.get_source_location(token),
        1, 1, 0
    );
DONE 

TEST(test_float_literal_decimal)
    const char *source = "123.456";
    Lexer lexer(source);
    auto token           = lexer.next_token();
    auto source_location = lexer.get_source_location(token);

    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_as(token.kind);
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_metadata_eq_to(
        source_location, 1, 1, 0
    );
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_str_eq_to(
        token, "123.456", STR_LEN(source)
    );
DONE 

TEST(test_float_literal_exponent)
    const char *source = "123456e10";
    Lexer lexer(source);
    auto token           = lexer.next_token();
    auto source_location = lexer.get_source_location(token);

    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_as(token.kind);
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_metadata_eq_to(
        source_location, 1, 1, 0
    );
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_str_eq_to(
        token, "123456e10", STR_LEN(source)
    );
DONE 

TEST(test_wrong_float_literal)
    const char *source = "-42e----49194";
    Lexer lexer(source);

    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();

    VERIFY_THIS(TokenKind::MINUS).token_as(t1.kind);
    VERIFY_THIS(TokenKind::MINUS).token_str_eq_to(t1, "-", STR_LEN("-"));
    VERIFY_THIS(TokenKind::MINUS).token_metadata_eq_to(
        lexer.get_source_location(t1), 1, 1, 0
    );
    
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_as(t2.kind);
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_str_eq_to(t2, "42e----49194", STR_LEN("42e----49194"));
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_metadata_eq_to(
        lexer.get_source_location(t2), 1, 2, 1
    );
DONE

TEST(test_float_literal_exponent_negative)
    const char *source = "-123456e-10";
    Lexer lexer(source);
    auto t1              = lexer.next_token();
    auto t2              = lexer.next_token();

    VERIFY_THIS(TokenKind::MINUS).token_as(t1.kind);
    VERIFY_THIS(TokenKind::MINUS).token_metadata_eq_to(
        lexer.get_source_location(t1), 1, 1, 0
    );
    VERIFY_THIS(TokenKind::MINUS).token_str_eq_to(
        t1, "-", STR_LEN("-")
    );

    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_as(t2.kind);
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_metadata_eq_to(
        lexer.get_source_location(t2), 1, 2, 1
    );
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_str_eq_to(
        t2, "123456e-10", STR_LEN("123456e-10")
    );
DONE 

TEST(fuzzing_int_literal_random)
    const int iterations = 100;

    for (int index = 0; index < iterations; ++index) {
        string number = rand_source_digits();
        Lexer lexer (number.c_str());

        auto token = lexer.next_token();

        VERIFY_THIS(TokenKind::INT_LITERAL).token_as(token.kind);
        VERIFY_THIS(TokenKind::INT_LITERAL).token_str_eq_to(
            token, number, number.length()
        );
        VERIFY_THIS(TokenKind::INT_LITERAL).token_metadata_eq_to(
            lexer.get_source_location(token),
            1, 1, 0
        );
    }
DONE 

TEST(fuzzing_float_literal_random)
    const int iterations = 100;

    for (int index = 0; index < iterations; ++index) {
        string number = rand_source_digits(false, false, true);
        Lexer lexer (number.c_str());

        auto token = lexer.next_token();

        VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_as(token.kind);
        VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_str_eq_to(
            token, number, number.length()
        );
        VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_metadata_eq_to(
            lexer.get_source_location(token),
            1, 1, 0
        );
    }
DONE

TEST(fuzzing_float_literal_exponent_random)
    const int iterations = 100;

    for (int index = 0; index < iterations; ++index) {
        string number = rand_source_digits(true, false, true);
        Lexer lexer(number.c_str());
        auto token = lexer.next_token();

        VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_as(token.kind);
        VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_metadata_eq_to(
            lexer.get_source_location(token),
            1, 1, 0
        );
        VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_str_eq_to(
            token, number, number.length()
        );
    }
DONE 


TEST(fuzzing_float_literal_nagative_exponent_random)
    const int iterations = 100;

    for (int index = 0; index < iterations; ++index) {
        string number = rand_source_digits(true, true, true);
        Lexer lexer(number.c_str());
        auto token = lexer.next_token();

        VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_as(token.kind);
        VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_metadata_eq_to(
            lexer.get_source_location(token),
            1, 1, 0
        );
        VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_str_eq_to(
            token, number, number.length()
        );
    }
DONE 

TEST(test_float_literal_undefined)
    const char *source = "-42e----49194-e4771";
    Lexer lexer(source);

    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();
    auto t3 = lexer.next_token();
    auto t4 = lexer.next_token();

    VERIFY_THIS(TokenKind::MINUS).token_as(t1.kind);
    VERIFY_THIS(TokenKind::MINUS).token_str_eq_to(t1, "-", STR_LEN("-"));
    VERIFY_THIS(TokenKind::MINUS).token_metadata_eq_to(
        lexer.get_source_location(t1), 1, 1, 0
    );
    
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_as(t2.kind);
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_str_eq_to(t2, "42e----49194", STR_LEN("42e----49194"));
    VERIFY_THIS(TokenKind::FLOAT_LITERAL).token_metadata_eq_to(
        lexer.get_source_location(t2), 1, 2, 1
    );

    VERIFY_THIS(TokenKind::MINUS).token_as(t3.kind);
    VERIFY_THIS(TokenKind::MINUS).token_str_eq_to(t3, "-", STR_LEN("-"));
    VERIFY_THIS(TokenKind::MINUS).token_metadata_eq_to(
        lexer.get_source_location(t3), 1, 14, 13
    );

    VERIFY_THIS(TokenKind::IDENT).token_as(t4.kind);
    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(t4, "e4771", STR_LEN("e4771"));
    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(
        lexer.get_source_location(t4), 1, 15, 14
    );
DONE 

TEST(test_normal_string_literal)
    const char *source = "\"My String\"";
    Lexer lexer(source);

    auto token = lexer.next_token();
    VERIFY_THIS(TokenKind::STRING_LITERAL).token_as(token.kind);
    VERIFY_THIS(TokenKind::STRING_LITERAL).token_str_eq_to(
        token,
        "\"My String\"", STR_LEN("\"My String\"")
    );
    VERIFY_THIS(TokenKind::STRING_LITERAL).token_metadata_eq_to(
        lexer.get_source_location(token),
        1, 1, 0
    );
DONE

TEST(test_string_literal_without_closed)
    const char *source = "\"My String";
    Lexer lexer(source);

    auto token = lexer.next_token();
    VERIFY_THIS(TokenKind::STRING_LITERAL).token_as(token.kind);
    VERIFY_THIS(TokenKind::STRING_LITERAL).token_str_eq_to(
        token,
        "\"My String", STR_LEN("\"My String")
    );
    VERIFY_THIS(TokenKind::STRING_LITERAL).token_metadata_eq_to(
        lexer.get_source_location(token),
        1, 1, 0
    );
DONE

TEST(test_normal_raw_string_literal)
    const char *source = "r\"My raw string\"";
    Lexer lexer(source);

    auto token = lexer.next_token();

    VERIFY_THIS(TokenKind::RAW_STRING_LITERAL).token_as(token.kind);
    VERIFY_THIS(TokenKind::RAW_STRING_LITERAL).token_str_eq_to(
        token,
        "r\"My raw string\"", STR_LEN("r\"My raw string\"")
    );
    VERIFY_THIS(TokenKind::RAW_STRING_LITERAL).token_metadata_eq_to(
        lexer.get_source_location(token),
        1, 1, 0
    );
DONE

TEST(test_raw_string_literal_without_closed)
    const char *source = "r\"My raw string";
    Lexer lexer(source);

    auto token = lexer.next_token();

    VERIFY_THIS(TokenKind::RAW_STRING_LITERAL).token_as(token.kind);
    VERIFY_THIS(TokenKind::RAW_STRING_LITERAL).token_str_eq_to(
        token,
        "r\"My raw string", STR_LEN("r\"My raw string")
    );
    VERIFY_THIS(TokenKind::RAW_STRING_LITERAL).token_metadata_eq_to(
        lexer.get_source_location(token),
        1, 1, 0
    );
DONE

TEST(test_special_ident_r)
    const char *source = "r";
    Lexer lexer(source);

    auto token = lexer.next_token();

    VERIFY_THIS(TokenKind::IDENT).token_as(token.kind);
    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(
        token,
        "r", STR_LEN("r")
    );
    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(
        lexer.get_source_location(token),
        1, 1, 0
    );
DONE

TEST_ENTRY
    RUN_TEST(test_int_literal)
    RUN_TEST(test_float_literal_decimal)
    RUN_TEST(test_float_literal_exponent)
    RUN_TEST(test_float_literal_exponent_negative)
    RUN_TEST(test_wrong_float_literal)
    RUN_TEST(test_float_literal_undefined)

    RUN_TEST(test_normal_string_literal)
    RUN_TEST(test_string_literal_without_closed)
    RUN_TEST(test_normal_raw_string_literal)
    RUN_TEST(test_raw_string_literal_without_closed)
    RUN_TEST(test_special_ident_r)

    RUN_TEST(fuzzing_int_literal_random)
    RUN_TEST(fuzzing_float_literal_random)
    RUN_TEST(fuzzing_float_literal_exponent_random)
    RUN_TEST(fuzzing_float_literal_nagative_exponent_random)
ENTRY_END