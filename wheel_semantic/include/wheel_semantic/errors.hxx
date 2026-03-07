#if !defined (SEMANTIC_ERRORS)
#define SEMANTIC_ERRORS
#include <cstdint>
#include "config.hxx"

WHEEL_SEMANTIC_BEGIN_NAMESPACE
    enum class SemanticErrorCode : uint16_t {
        DUPLICATE_SYMBOL        = 3001,
        UNSUPPORTED_TYPE        = 3002,
        TYPE_MISMATCH           = 3003,
        INVALID_STRING_LITERAL  = 3004,
        INVALID_INTEGER_LITERAL = 3005,

        UnsupportedNode         = 3006,
        InvalidInitializer      = 3007,
        UnsupportedFunction     = 3008,
        UndefinedSymbol         = 3009,
        InvalidFormatString     = 3010,
        InvalidArgumentCount    = 3011,
        FormatArgumentMismatch  = 3012,
        UnsupportedExpression   = 3013,
        INTEGER_OUT_OF_RANGE    = 3014
    };

    [[nodiscard]]
    inline const char* from_senematic_err(SemanticErrorCode code) noexcept {
        using S = SemanticErrorCode;

        switch (code) {
            case S::DUPLICATE_SYMBOL: return "DUPLICATE_SYMBOL";
            case S::UNSUPPORTED_TYPE: return "UNSUPPORTED_TYPE";
            case S::INVALID_INTEGER_LITERAL: return "INVALID_INTEGER_LITERAL";
            
            default: return "UNSUPPORTED_NODE";
        }
    }

WHEEL_SEMANTIC_END_NAMESPACE

#endif // SENEMATIC_ERRORS