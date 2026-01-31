#if !defined(ALIASES_HXX)
#define ALIASES_HXX

#include <string_view>
#include <cstddef>

#if defined(__clang__) || defined(__GNUC__)
    #define WHEEL_ALWAYS_INLINE                             __attribute__((always_inline))
    #define WHEEL_COMPTIME_NODISCARD_INLINE [[nodiscard]]   __attribute__((always_inline)) constexpr
    #define WHEEL_ALWAYS_INLINE_NODISCARD  [[nodiscard]]   __attribute__((always_inline))

#elif defined(_MSC_VER)
    #define WHEEL_ALWAYS_INLINE                                          __forceinline
    #define WHEEL_COMPTIME_NODISCARD_INLINE                [[nodiscard]] __forceinline constexpr
    #define WHEEL_ALWAYS_INLINE_NODISCARD                  [[nodiscard]] __forceinline
#else
    #warning  "WHEEL_ALWAYS_INLINE macro is not supported on this compiler. Peformance may be degraded."
#endif 

using StringView = std::string_view;
using SizeT      = std::size_t;

#endif // ALIASES_HXX