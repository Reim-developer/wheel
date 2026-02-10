#if !defined(CONFIG_HXX)
#define CONFIG_HXX

#define WHEEL_PARSER_NAMESPACE namespace wheel_parser {
#define WHEEL_PARSER_END_NAMESPACE }

#if defined(_MSC_VER)
    #define WHEEL_NO_INLINE __declspec(noinline)

#elif defined(__clang__) || defined(__GNUC__)
    #define WHEEL_NO_INLINE __attribute__((noinline))
#else 
    #warning "WHEEL_NO_INLINE macro is not supported on this compiler."
#endif

#endif // CONFIG_HXX