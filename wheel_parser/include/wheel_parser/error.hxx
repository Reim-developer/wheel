#if !defined(PARSER_ERROR_HXX)
#define PARSER_ERROR_HXX
#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include <string>

#if !defined (WHEEL_EXPERIMENT) && !defined (WHEEL_SMALL_VEC)
    #include <vector>
#endif

#include <wheel_lexer/kind.hxx>
#include <wheel_lexer/token.hxx>
#include <wheel_memory/vec.hxx>
#include "config.hxx"

using wheel_lexer::SourceLocation;
using wheel_lexer::Token;
using wheel_lexer::TokenKind;

WHEEL_PARSER_NAMESPACE
    enum class ParseErrorCode : uint16_t {
        ExpectedIdentifier              = 1001,
        ExpectedColon                   = 1002,
        ExpectedType                    = 1003,
        ExpectedEqual                   = 1004,
        ExpectedLiteral                 = 1005,
        UnexpectedStatement             = 1006,
        ExpectedTypeKeyword             = 1007,
        ExpectedLeftParent              = 1008,
        ExpectedRightParent             = 1009,
        ExpectedExpression              = 1010,
        ExpectedArgumentSeparator       = 1011,

        ExpectedFunctionName            = 1012,
        ExpectedFunctionKeyword         = 1013,
        ExpectedIdentifierAfterFunction = 1014,
        ExpectedParameterList           = 1015,
        ExpectedReturnType              = 1016,
        ExpectedFunctionBody            = 1017,
        ExpectedFunctionArgs            = 1018,
        ExpectedFunctionArgsType        = 1019
    };

    struct ParseError {
        ParseErrorCode   code;
        const Token      *token;
        TokenKind        expected;
        TokenKind        actual;
        SourceLocation   location;
        std::string      message;
    };

    struct ParseDiagnostic {
        ParseError   error;
        std::string  full_message;
        std::string  source_line;
        std::string  marker;
    };

    #if defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
        using ParseDiagnosticList = wheel_memory::SmallVec<ParseDiagnostic>;
    #else
        using ParseDiagnosticList = std::vector<ParseDiagnostic>;
    #endif

    [[nodiscard]] inline ParseError make_parse_error(
        ParseErrorCode code,
        const Token *token,
        TokenKind actual,
        SourceLocation location,
        std::string message = {},
        TokenKind expected = TokenKind::IDENT
    ) noexcept {
        return ParseError {
            code,
            token,
            expected,
            actual,
            location,
            std::move(message)
        };
    }

    [[nodiscard]] inline std::string format_parse_error(
        const ParseError &error,
        std::string_view file_name = {}
    ) {
        return std::format(
            "[P{:04}] [{}:{}:{}] {} (expected: {}, got: {})",
            static_cast<uint16_t>(error.code),
            file_name.empty() ? "unknown" : file_name,
            error.location.line,
            error.location.column,
            error.message,
            wheel_lexer::to_string(error.expected),
            wheel_lexer::to_string(error.actual)
        );
    }

    [[nodiscard]] inline ParseDiagnostic build_parse_diagnostic(
        const ParseError &error,
        std::string_view source_line,
        std::string_view file_name = {}
    ) {
        const size_t marker_spaces = (error.location.column > 0)
            ? static_cast<size_t>(error.location.column - 1)
            : 0;

        size_t marker_length = 1;
        if (error.token != nullptr) {
            const size_t token_len = static_cast<size_t>(error.token->end - error.token->start);
            marker_length = (token_len > 0) ? token_len : 1;
        }

        ParseDiagnostic diagnostic;
        diagnostic.error = error;
        diagnostic.full_message = format_parse_error(error, file_name);
        diagnostic.source_line = std::string(source_line);
        diagnostic.marker = std::string(marker_spaces, ' ') + std::string(marker_length, '^');

        return diagnostic;
    }

WHEEL_PARSER_END_NAMESPACE

#endif // PARSER_ERROR_HXX
