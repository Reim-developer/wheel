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

            constexpr Token() noexcept : kind(__Kind::EOF_), start(0), end(0) {}

            constexpr Token(__Kind kind, StringView str, 
                        size_t start, size_t end) noexcept : 
                        kind(kind), str(str), start(start),
                        end(end) {}
        
        [[nodiscard]] inline bool is(__Kind kind_) const noexcept {
            return kind == kind_;
        }
        
        [[nodiscard]] inline bool is_eof() const noexcept {
            return kind == TokenKind::EOF_;
        }

        [[nodiscard]] inline bool is_one_of(__Kind kind1, __Kind kind2) const noexcept {
            return kind == kind1 || kind == kind2;
        }

       
        [[nodiscard]] inline bool text_is(StringView str_) const noexcept {
            return str == str_;
        }
    };


    [[nodiscard]] inline Token make_token(__Kind kind, StringView str, 
                    size_t start, size_t end) {
        
        return Token {
            kind, str, start, end
        };
    }

    [[nodiscard]] inline Token make_eof(size_t pos) {
        return Token {
            TokenKind::EOF_,
            "", pos, pos
        };
    }

    [[nodiscard]] inline constexpr Token make_identifier_fallback(size_t pos, StringView source_text) {
        return Token {
            TokenKind::IDENT,
            source_text,
            pos, pos + 1
        };
    }

END_NAMESPACE

#endif // TOKEN_HXX