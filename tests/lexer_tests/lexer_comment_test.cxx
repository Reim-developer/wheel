#include "__lexer_test_helper/__helper.hxx"
#include <wheel_lexer/lexer.hxx>

using wheel_lexer::Lexer;
using wheel_lexer::TokenKind;

TEST(test_normal_block_comment)
    const char* source = "/* comment */";

    Lexer lexer(source);
    auto token = lexer.next_token();

    VERIFY_THIS(TokenKind::COMMENT).token_str_eq_to(token, "/* comment */", STR_LEN(source));
    VERIFY_THIS(TokenKind::COMMENT).token_metadata_eq_to(token, 1, 14, 0);
    VERIFY_THIS(TokenKind::COMMENT).token_as(token.kind);
DONE 

TEST(test_normal_document_block_comment)
    const char *source = "/// simple document";

    Lexer lexer(source);
    auto token = lexer.next_token();

    VERIFY_THIS(TokenKind::DOCUMENT_COMMENT).token_str_eq_to(token, "/// simple document", STR_LEN(source));
    VERIFY_THIS(TokenKind::DOCUMENT_COMMENT).token_metadata_eq_to(token, 1, 20, 0);
    VERIFY_THIS(TokenKind::DOCUMENT_COMMENT).token_as(token.kind);
DONE

TEST(test_normal_comment_with_newline)
    const char *source = "/* \n my multiple line comment */";

    Lexer lexer(source);
    auto token = lexer.next_token();

    VERIFY_THIS(TokenKind::COMMENT).token_str_eq_to(token, "/* \n my multiple line comment */", STR_LEN(source));
    VERIFY_THIS(TokenKind::COMMENT).token_metadata_eq_to(token, 2, 29, 0);
    VERIFY_THIS(TokenKind::COMMENT).token_as(token.kind);
DONE

TEST(test_comment_error)
    const char *source = "/* bad comment without close";

    Lexer lexer(source);
    auto token = lexer.next_token();

    VERIFY_THIS(TokenKind::COMMENT).token_str_eq_to(token, "/* bad comment without close", STR_LEN(source));
    VERIFY_THIS(TokenKind::COMMENT).token_as(token.kind);
    VERIFY_THIS(TokenKind::COMMENT).token_metadata_eq_to(token, 1, 29, 0);
DONE

TEST(test_division_operator_basic)
    const char *source = "a / b";

    Lexer lexer(source);
    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();
    auto t3 = lexer.next_token();
    auto t4 = lexer.next_token();
    auto t5 = lexer.next_token();

    VERIFY_THIS(TokenKind::IDENT).token_as(t1.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t2.kind);
    VERIFY_THIS(TokenKind::SLASH).token_as(t3.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t4.kind);
    VERIFY_THIS(TokenKind::IDENT).token_as(t5.kind);
DONE 

TEST(test_division_vs_comment_like)
    const char *source = "a / b // just a simple comment";
    Lexer lexer(source);
    
    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();
    auto t3 = lexer.next_token();
    auto t4 = lexer.next_token();
    auto t5 = lexer.next_token();
    auto t6 = lexer.next_token();
    auto t7 = lexer.next_token();

    VERIFY_THIS(TokenKind::IDENT).token_as(t1.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t2.kind);
    VERIFY_THIS(TokenKind::SLASH).token_as(t3.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t4.kind);
    VERIFY_THIS(TokenKind::IDENT).token_as(t5.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t6.kind);
    VERIFY_THIS(TokenKind::COMMENT).token_as(t7.kind);
DONE 

TEST(test_division_vs_comment_block_like)
    const char *source = "a / b /* just a simple comment */";
    Lexer lexer(source);
    
    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();
    auto t3 = lexer.next_token();
    auto t4 = lexer.next_token();
    auto t5 = lexer.next_token();
    auto t6 = lexer.next_token();
    auto t7 = lexer.next_token();

    VERIFY_THIS(TokenKind::IDENT).token_as(t1.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t2.kind);
    VERIFY_THIS(TokenKind::SLASH).token_as(t3.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t4.kind);
    VERIFY_THIS(TokenKind::IDENT).token_as(t5.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t6.kind);
    VERIFY_THIS(TokenKind::COMMENT).token_as(t7.kind);
DONE

TEST(test_division_vs_comment_document_like)
    const char *source = "a / b /// Just something document comment";
    Lexer lexer(source);
    
    auto t1 = lexer.next_token();
    auto t2 = lexer.next_token();
    auto t3 = lexer.next_token();
    auto t4 = lexer.next_token();
    auto t5 = lexer.next_token();
    auto t6 = lexer.next_token();
    auto t7 = lexer.next_token();

    VERIFY_THIS(TokenKind::IDENT).token_as(t1.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t2.kind);
    VERIFY_THIS(TokenKind::SLASH).token_as(t3.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t4.kind);
    VERIFY_THIS(TokenKind::IDENT).token_as(t5.kind);
    VERIFY_THIS(TokenKind::SPACE).token_as(t6.kind);
    VERIFY_THIS(TokenKind::DOCUMENT_COMMENT).token_as(t7.kind);
DONE

TEST_ENTRY
    RUN_TEST(test_normal_block_comment)
    RUN_TEST(test_normal_document_block_comment)
    RUN_TEST(test_normal_comment_with_newline)
    RUN_TEST(test_comment_error)
    RUN_TEST(test_division_operator_basic)
    RUN_TEST(test_division_vs_comment_like)
    RUN_TEST(test_division_vs_comment_block_like)
    RUN_TEST(test_division_vs_comment_document_like)
ENTRY_END