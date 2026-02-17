#if !defined (WHEEL_MEMORY_CONFIG_HXX)
#define WHEEL_MEMORY_CONFIG_HXX

#define WHEEL_MEMORY_NAMESPACE namespace wheel_memory {
#define WHEEL_MEMORY_END_NAMESPACE }
#define WHEEL_MEMORY_NAMESPACE_NAME wheel_memory

#if defined (WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
    #define HAS_USE_SMALL_VEC 1

#elif !defined(WHEEL_EXPERIMENT) && defined(WHEEL_SMALL_VEC)
    #error "Cannot use WHEEL_SMALL_VEC without WHEEL_EXPERIMENT"
#else 
    #define HAS_USEHAS_USE_SMALL_VEC 0
#endif 

#if defined(_MSC_VER)
    #define WHEEL_NO_INLINE __declspec(noinline)

#elif defined(__clang__) || defined(__GNUC__)
    #define WHEEL_NO_INLINE __attribute__((noinline))
#else 
    #warning "WHEEL_NO_INLINE macro is not supported on this compiler."
#endif

#if defined(WHEEL_EXPERIMENT)
    #if defined(WHEEL_SMALL_VEC)
        #define USE_WHEEL_SMALL_VEC using WHEEL_MEMORY_NAMESPACE_NAME::SmallVec;
    #else
        #define USE_WHEEL_SMALL_VEC static_assert(true, "WHEEL_SMALL_VEC has been disabled.");
    #endif 
    
#else 
    #define USE_WHEEL_SMALL_VEC static_assert(true, "WHEEL_EXPERIMENT && WHEEL_SMALL_VEC has been disabled.")
#endif 

#endif // WHEEL_MEMORY_CONFIG_HXX