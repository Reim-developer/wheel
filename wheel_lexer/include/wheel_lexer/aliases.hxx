#if !defined(ALIASES_HXX)
#define ALIASES_HXX

#include <string_view>
#include <cstddef>

#define WHEEL_STR_LIKELY(str1, str2)   (str1 == str2) [[likely]]
#define WHEEL_STR_UNLIKELY(str1, str2) (str1 != str2) [[unlikely]]

#if defined(__clang__) || defined(__GNUC__)
    #define WHEEL_ALWAYS_INLINE                             __attribute__((always_inline)) inline
    #define WHEEL_COMPTIME_NODISCARD_INLINE [[nodiscard]]   __attribute__((always_inline)) inline constexpr
    #define WHEEL_ALWAYS_INLINE_NODISCARD   [[nodiscard]]   __attribute__((always_inline))  inline

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