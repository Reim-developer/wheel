#if !defined (__HELPER_HXX)
#define __HELPER_HXX

#include <lexer/lexer.hxx>
#include <lexer/kind.hxx>
#include <string_view>
#include <source_location>
#include <format>
#include <iostream>
#include <filesystem>
#include <optional>
#include <cstddef>

#define CURRENT std::source_location::current()
#define TEST(name) void name() { \
    std::cout << std::format("[TEST] '{}'", #name) << "\n";

#define DONE }
#define RUN_TEST(test) test();

#define TEST_ENTRY int main() { 

#define ENTRY_END return  0; }

using TokenKind         = lexer::TokenKind;
using Token             = lexer::Token;
using StdSourceLocation = std::source_location;
using StringView        = std::string_view;
using Path              = std::filesystem::path;

template<TokenKind ExpectedKind>
struct Verify {
    private:
        StdSourceLocation m_source_location;
        bool              m_output_only;

    public:
        Verify(bool output_only = false, StdSourceLocation current = CURRENT) : 
            m_output_only(output_only),
            m_source_location(current) {}

        void token_metadata_eq_to(const Token &token, 
                                std::size_t file_line, std::size_t file_column, 
                                std::size_t file_offset) {
            const auto basename = Path(m_source_location.file_name()).filename().string();
            const auto line     = m_source_location.line();
            
            if (!m_output_only) {
                if (token.source_location.line != file_line 
                    || token.source_location.column != file_column 
                    || token.source_location.offset != file_offset) {
                    std::cerr << std::format(
                        "[FAILED] [{}:{}] Assertion Error. Expect: |{}, {}, {}|. Got: |{}, {}, {}|",
                        line, basename, 
                        file_line, file_column, file_offset,
                        token.source_location.line,
                        token.source_location.column,
                        token.source_location.offset
                    ) << "\n";
                    std::abort();
                }
            }

            std::cout<< std::format(
                "[SUCCESS] [{}:{}] Expected Source Metadata |{}, {}, {}|. Got: |{}, {}, {}|",
                line, basename, 
                file_line, file_column, file_offset,
                token.source_location.line,
                token.source_location.column,
                token.source_location.offset
            ) << "\n";
        }

        void token_str_eq_to(const Token &token, StringView expected_text, 
                            std::optional<size_t> expected_length = std::nullopt) {
            const auto basename = Path(m_source_location.file_name()).filename().string();
            const auto line     = m_source_location.line();
            
            if (!m_output_only) {
                if(token.str != expected_text) {
                    std::cerr << std::format(
                        "[FAILED] [{}:{}] Assertion Error: '{}' is not equal to '{}'",
                        line, basename, 
                        token.str, expected_text
                    ) << "\n";
                    std::abort();
                }
            }

            std::cout << std::format(
                "[SUCCESS] [{}:{}] Expected Token String Equal To: '{}'. Got: '{}'",
                line, basename,
                token.str, expected_text
            ) << "\n";

            if (expected_length.has_value() && !m_output_only) {
                if(token.str.length() != expected_length) {
                    std::cerr << std::format(
                        "[FAILED] [{}:{}] Assertion Error: '{}' is not equal to '{}'",
                        line, basename, 
                        token.str.length(), expected_length.value()
                    ) << "\n";
                    std::abort();
                }

                std::cout << std::format(
                    "[SUCCESS] [{}:{}] Expected Token Length Equal To: '{}'. Got: '{}'",
                    line, basename,
                    token.str.length(), expected_length.value()
                ) << "\n";
            }
        }

        void token_as(TokenKind token_kind) {
            const auto basename = Path(m_source_location.file_name()).filename().string();
            const auto line     = m_source_location.line();
            
            if (!m_output_only) {
                if(token_kind != ExpectedKind) {
                    std::cerr << std::format(
                        "[FAILED] [{}:{}] Assertion Error: {} - ('{}') is not equal of {} - ('{}')",
                        line, basename, 
                        static_cast<int>(token_kind), lexer::to_string(token_kind),
                        static_cast<int>(ExpectedKind), lexer::to_string(ExpectedKind)
                    ) << "\n";
                    std::abort();
                }
            }

            std::cout << std::format(
                "[SUCCESS] [{}:{}] Expected Token Enum As: {} - ('{}'). Got: {} - ('{}')",
                line, basename,
                static_cast<int>(token_kind),
                lexer::to_string(token_kind),
                static_cast<int>(ExpectedKind),
                lexer::to_string(ExpectedKind)
            ) << "\n";
        }
};

#endif // __HELPER_HXX