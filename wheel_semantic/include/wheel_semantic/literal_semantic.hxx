#if !defined (LITERAL_LIMITS_HXX)
#define LITERAL_LIMITS_HXX
#include "errors.hxx"
#include "config.hxx"
#include <string_view>
#include <cstdint>
#include <optional>
#include <charconv>
#include <system_error>
#include <wheel_utils/logging.hxx>


using wheel_semantic::SemanticErrorCode;
WHEEL_SEMANTIC_BEGIN_NAMESPACE
    #define return_and_debug(source, code) \
       do { \
            DEBUG_PRINT( \
                FORMAT("[{}] {}", \
                    from_senematic_err(code), \
                        source \
                    ) \
            ); \
            return code; \
        } while(0)

    inline std::optional<SemanticErrorCode> semantic_integer_literal(std::string_view source, int64_t& value) {
        using S = SemanticErrorCode;

        const char *begin = source.data();
        const char *end = begin + source.size();
        const auto [parsed_until, error] = std::from_chars(begin, end, value);

        if (parsed_until != end) {
            return_and_debug(source, S::INVALID_INTEGER_LITERAL);
        }

        if (error == std::errc{}) {
            return std::nullopt;
        }

        switch (error) {
            case std::errc::result_out_of_range: return_and_debug(source, S::INTEGER_OUT_OF_RANGE);
            case std::errc::invalid_argument: return_and_debug(source, S::INVALID_INTEGER_LITERAL);

            default: return_and_debug(source, SemanticErrorCode::INVALID_INTEGER_LITERAL);
        }

        return std::nullopt;
    }

WHEEL_SEMANTIC_END_NAMESPACE

#endif // LITERAL_LIMITS_HXX