#if !defined(LOGGING_HXX)
#define LOGGING_HXX
#include "__wheel_utils_config.hxx"

#include <iostream>
#include <format>
#include <cstring>

WHEEL_UTILS_NAMESPACE 

#if defined(WHEEL_DEBUG)
    #if defined(_MSVC_LANG)
        #define CURRENT_CPP_VERSION _MSVC_LANG

    #else 
        #define CURRENT_CPP_VERSION __cplusplus
    #endif 

    #if CURRENT_CPP_VERSION >= 202002L
        #if defined(_WIN32)
            #define WHEEL_BASE_NAME(path) ((std::strrchr(path, '\\')) ? std::strrchr(path, '\\') + 1 : path)

        #else
            #define WHEEL_BASE_NAME(path) ((std::strrchr(path, '/')) ? std::strrchr(path, '/') + 1 : path)

        #endif 

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