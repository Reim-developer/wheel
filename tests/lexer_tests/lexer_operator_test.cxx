#include "__lexer_test_helper/__helper.hxx"
#include <__internal/__assert.hxx>
#include <wheel_lexer/lexer.hxx>
#include <wheel_lexer/token.hxx>

using wheel_lexer::Lexer;
using wheel_lexer::TokenKind;
using wheel_lexer::Token;

struct Metadata {
    size_t line;
    size_t column;
    size_t offset;
};

template<TokenKind kind>
void operator_expected(Token &token, 
        SourceLocation &source_location, Metadata metadata, const char* expected_str) {
    VERIFY_THIS(kind).token_str_eq_to(token, expected_str, STR_LEN(expected_str));
    VERIFY_THIS(kind).token_metadata_eq_to(source_location, metadata.line, metadata.column, metadata.offset);
    VERIFY_THIS(kind).token_as(token.kind);
}

template<TokenKind kind>
void operator_statement_expected(char operator_) {
    std::string statement_fmt = std::format("a {} b = c", operator_);
    const char* statement    = statement_fmt.c_str();

    Lexer lexer(statement);
    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();
    auto t3 = lexer.next_token();
    auto t4 = lexer.next_token();
    auto t5 = lexer.next_token();
    auto t6 = lexer.next_token();
    auto t7 = lexer.next_token();
    auto t8 = lexer.next_token();
    auto t9 = lexer.next_token();

    VERIFY_THIS(TokenKind::IDENT).token_as(t1.kind);
    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(lexer.get_source_location(t1), 1, 1, 0);
    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(t1, "a", 1);

    VERIFY_THIS(TokenKind::SPACE).token_as(t2.kind);
    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(lexer.get_source_location(t2), 1, 2, 1);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t2, " ", 1);

    VERIFY_THIS(kind).token_as(t3.kind);
    VERIFY_THIS(kind).token_metadata_eq_to(lexer.get_source_location(t3), 1, 3, 2);
    VERIFY_THIS(kind).token_str_eq_to(t3, std::string_view(std::string(1, operator_)), 1);

    VERIFY_THIS(TokenKind::SPACE).token_as(t4.kind);
    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(lexer.get_source_location(t4), 1, 4, 3);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t4, " ", 1);

    VERIFY_THIS(TokenKind::IDENT).token_as(t5.kind);
    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(lexer.get_source_location(t5), 1, 5, 4);
    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(t5, "b", 1);

    VERIFY_THIS(TokenKind::SPACE).token_as(t6.kind);
    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(lexer.get_source_location(t6), 1, 6, 5);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t6, " ", 1);

    VERIFY_THIS(TokenKind::EQUAL).token_as(t7.kind);
    VERIFY_THIS(TokenKind::EQUAL).token_metadata_eq_to(lexer.get_source_location(t7), 1, 7, 6);
    VERIFY_THIS(TokenKind::EQUAL).token_str_eq_to(t7, "=", 1);

    VERIFY_THIS(TokenKind::SPACE).token_as(t8.kind);
    VERIFY_THIS(TokenKind::SPACE).token_metadata_eq_to(lexer.get_source_location(t8), 1, 8, 7);
    VERIFY_THIS(TokenKind::SPACE).token_str_eq_to(t8, " ", 1);

    VERIFY_THIS(TokenKind::IDENT).token_as(t9.kind);
    VERIFY_THIS(TokenKind::IDENT).token_metadata_eq_to(lexer.get_source_location(t9), 1, 9, 8);
    VERIFY_THIS(TokenKind::IDENT).token_str_eq_to(t9, "c", 1);

}

TEST(test_normal_minus_operator)
    const char *source = "-";
    Lexer lexer(source);

    auto token = lexer.next_token();
    auto source_location  = lexer.get_source_location(token);
    operator_expected<TokenKind::MINUS>(token, source_location, Metadata {
        .line   = 1,
        .column = 1,
        .offset = 0
    }, "-");
DONE 

TEST(test_normal_minus_minus_operator)
    const char *source = "--";
    Lexer lexer(source);

    auto token            = lexer.next_token();
    auto source_location  = lexer.get_source_location(token);
    operator_expected<TokenKind::MINUS_MINUS>(token, source_location, Metadata {
        .line   = 1,
        .column = 1,
        .offset = 0
    }, "--");
DONE 

TEST(test_normal_plus_operator)
    const char *source = "+";
    Lexer lexer(source);

    auto token = lexer.next_token();
    auto source_location  = lexer.get_source_location(token);
    operator_expected<TokenKind::PLUS>(token, source_location, Metadata {
        .line   = 1,
        .column = 1,
        .offset = 0
    }, "+");
DONE 

TEST(test_normal_plus_plus_operator)
    const char *source = "++";
    Lexer lexer(source);

    auto token = lexer.next_token();
    auto source_location  = lexer.get_source_location(token);
    operator_expected<TokenKind::PLUS_PLUS>(token, source_location, Metadata {
        .line   = 1,
        .column = 1,
        .offset = 0
    }, "++");
DONE 

TEST(test_normal_star_operator)
    const char *source = "*";
    Lexer lexer(source);

    auto token = lexer.next_token();
    auto source_location  = lexer.get_source_location(token);
    operator_expected<TokenKind::STAR>(token, source_location, Metadata {
        .line   = 1,
        .column = 1,
        .offset = 0
    }, "*");
DONE 

TEST(test_normal_slash_operator)
    const char *source = "/";
    Lexer lexer(source);

    auto token = lexer.next_token();
    auto source_location  = lexer.get_source_location(token);
    operator_expected<TokenKind::SLASH>(token, source_location, Metadata {
        .line   = 1,
        .column = 1,
        .offset = 0
    }, "/");
DONE 

TEST(test_normal_arrow_operator)
    const char *source = "->";
    Lexer lexer(source);

    auto token              = lexer.next_token();
    auto source_location    = lexer.get_source_location(token);
    operator_expected<TokenKind::ARROW>(token, source_location, Metadata {
        .line   = 1,
        .column = 1,
        .offset = 0
    }, "->");
DONE 

TEST(test_normal_fat_arrow_operator)
    const char *source = "=>";
    Lexer lexer(source);

    auto token              = lexer.next_token();
    auto source_location    = lexer.get_source_location(token);
    operator_expected<TokenKind::FAT_ARROW>(token, source_location, Metadata {
        .line   = 1,
        .column = 1,
        .offset = 0
    }, "=>");
DONE 

TEST(test_normal_operator_with_simple_statement)
   operator_statement_expected<TokenKind::MINUS>('-');
   operator_statement_expected<TokenKind::PLUS>('+');
   operator_statement_expected<TokenKind::SLASH>('/');
   operator_statement_expected<TokenKind::STAR>('*');
DONE 

TEST_ENTRY
    RUN_TEST(test_normal_minus_operator)
    RUN_TEST(test_normal_minus_minus_operator)
    RUN_TEST(test_normal_plus_operator)
    RUN_TEST(test_normal_plus_plus_operator)
    RUN_TEST(test_normal_star_operator)
    RUN_TEST(test_normal_slash_operator)
    RUN_TEST(test_normal_operator_with_simple_statement);
    RUN_TEST(test_normal_arrow_operator);
    RUN_TEST(test_normal_fat_arrow_operator);
ENTRY_END
