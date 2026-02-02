#if !defined(TOKEN_HXX)
#define TOKEN_HXX

#include "kind.hxx"
#include "aliases.hxx"
#include "__wheel_lexer_config.hxx"

WHEEL_LEXER_NAMESPACE
    using __Kind = TokenKind;

    struct SourceLocation {
        SizeT line   = 1;
        SizeT column = 1;
        SizeT offset = 0;
    };

    struct Token {
        public:
            TokenKind       kind;
            StringView      str;
            SizeT           start;
            SizeT           end;
            SourceLocation  source_location;

            constexpr Token() noexcept : kind(__Kind::EOF_), start(0), end(0) {}

            constexpr Token(__Kind kind, StringView str, 
                        size_t start, size_t end) noexcept : 
                        kind(kind), str(str), start(start),
                        end(end) {}

            constexpr Token(__Kind kind, StringView str, 
                        size_t start, size_t end,
                        SourceLocation source_location) noexcept : 
                        kind(kind), str(str), start(start),
                        end(end), source_location(source_location) {}
        
        [[nodiscard]] bool is(__Kind kind_) const noexcept {
            return kind == kind_;
        }
        
        [[nodiscard]] bool is_eof() const noexcept {
            return kind == TokenKind::EOF_;
        }

        [[nodiscard]] bool is_error() const noexcept {
            return kind == TokenKind::ERROR;
        }

        [[nodiscard]] bool is_one_of(__Kind kind1, __Kind kind2) const noexcept {
            return kind == kind1 || kind == kind2;
        }

       
        [[nodiscard]] bool text_is(StringView str_) const noexcept {
            return str == str_;
        }
    };


    [[nodiscard]] Token make_token(__Kind kind, StringView str, 
                    size_t start, size_t end) {
        
        return Token {
            kind, str, start, end
        };
    }

    [[nodiscard]] Token make_eof(size_t pos) {
        return Token {
            TokenKind::EOF_,
            "", pos, pos
        };
    }

    [[nodiscard]] constexpr Token make_error(size_t pos, StringView error_details) {
        return Token {
            TokenKind::ERROR,
            error_details,
            pos, pos + 1
        };
    }

END_NAMESPACE

#endif // TOKEN_HXX