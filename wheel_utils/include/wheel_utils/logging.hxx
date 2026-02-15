#if !defined(LOGGING_HXX)
#define LOGGING_HXX
#include "__wheel_utils_config.hxx"

#include <iostream>     // IWYU pragma: keep
#include <format>       // IWYU pragma: keep
#include <cstring>      // IWYU pragma: keep

WHEEL_UTILS_NAMESPACE 

 #if defined(_MSVC_LANG)
    #define CURRENT_CPP_VERSION _MSVC_LANG

#else 
    #define CURRENT_CPP_VERSION __cplusplus
#endif

#if defined(__clang__) || defined(__GNUC__)
    #define WHEEL_ALWAYS_INLINE                             __attribute__((always_inline)) inline

#elif defined(_MSC_VER)
    #define WHEEL_ALWAYS_INLINE                             __forceinline
#else
    #warning  "WHEEL_ALWAYS_INLINE macro is not supported on this compiler. Peformance may be degraded."
#endif 

 #if defined(_WIN32)
    #define WHEEL_BASE_NAME(path) ((std::strrchr(path, '\\')) ? std::strrchr(path, '\\') + 1 : path)

    #else
        #define WHEEL_BASE_NAME(path) ((std::strrchr(path, '/')) ? std::strrchr(path, '/') + 1 : path)
#endif

#if !defined (WHEEL_NO_FAIL_FAST)
    #define NOT_NULL_CONTEXT void

#else 
    #define NOT_NULL_CONTEXT bool
#endif 

#if defined(WHEEL_ASSERTION)
    #if CURRENT_CPP_VERSION < 202002L && defined (WHEEL_SOURCE_LOCATION)
        #error "WHEEL_SOURCE_LOCATION only supports C++ version >= 20"
    #endif 

    #if defined (WHEEL_SOURCE_LOCATION)
        #include <type_traits> 
        #include <source_location>

        template<typename T>
        requires ::std::is_pointer_v<T>
        WHEEL_ALWAYS_INLINE void not_null(T ptr, const std::source_location current = std::source_location::current()) {
            if (ptr == nullptr) [[unlikely]] {
                const auto info_fmt = ::std::format(
                    "[NULL_CRITICAL] [{}:{}:{}]",
                    current.file_name(), current.function_name(),
                    current.line()
                );
                ::std::cerr << ::std::format("{} Null pointer is not allowed in this context.\n", info_fmt);
                ::std::abort();
            }
        }
    #endif  

    #if defined (WHEEL_EXPERIMENT)
        template<typename T>
        struct is_pointer {
            static const bool value = false;
        };
        
        template<typename T>
        struct is_pointer<T*> {
            static const bool value = true;
        };

        template<typename T>
        inline constexpr bool is_pointer_value = is_pointer<T>::value;

        template<typename T>
        WHEEL_ALWAYS_INLINE NOT_NULL_CONTEXT not_null(T ptr) {
            static_assert(is_pointer_value<T> == true, "T must be a pointer");

            if (ptr == nullptr) {
                ::std::cerr << "Null pointer is not allowed in this context.\n";

                #if !defined (WHEEL_NO_FAIL_FAST)
                    ::std::abort();

                #else 
                    return false;
                #endif
            }

            #if defined (WHEEL_NO_FAIL_FAST)
                return true;
            #endif 
        }
    #endif 
    
    #if CURRENT_CPP_VERSION >= 202002L && !defined(WHEEL_SOURCE_LOCATION) && !defined (WHEEL_EXPERIMENT)
        #include <type_traits>

        template<typename T>
        requires ::std::is_pointer_v<T>
        WHEEL_ALWAYS_INLINE void not_null(T ptr) {
            if (ptr == nullptr) [[unlikely]] {
                ::std::cerr << "Null pointer is not allowed in this context.\n";
                ::std::abort();
            }
         }
    #endif 
#endif

#if defined(WHEEL_DEBUG) 
    #if CURRENT_CPP_VERSION >= 202002L 
        #define DEBUG_PRINT(msg) \
            do { \
                const auto fmt = std::format("[DEBUG] [{}:{}] {}", __LINE__, WHEEL_BASE_NAME(__FILE__), msg);\
                std::cout << fmt << "\n";\
            } while(0)

        #define FORMAT(...) std::format(__VA_ARGS__)
    
    #else
        #define DEBUG_PRINT(msg) ((void)0)
        #define FORMAT(...) ((void)0)
    #endif

#else 
    #define DEBUG_PRINT(msg) ((void)0)
    #define FORMAT(...) ((void)0)
#endif 

WHEEL_UTILS_END_NAMESPACE

#endif // LOGGING_HXX