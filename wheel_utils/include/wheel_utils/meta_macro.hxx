#if !defined (META_MACRO_HXX)

#define WHEEL_EAT_ALL_IF(condition, func) \
    do { \
        while(true) { \
            func(); \
            if (condition) break; \
        } \
    } while(0)

#endif // META_MACRO_HXX