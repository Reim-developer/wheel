#ifndef __ASSERT_HXX
#define __ASSERT_HXX

#include <iostream>
#include <cstdlib>
#include <cstring>

using std::cerr;
using std::cout;
using std::boolalpha;
using std::abort;

#if defined(_MSC_VER)
    #pragma warning(disable: 4806)
#endif 

#if defined(_WIN32)
    #define BASE_NAME(path) ((strrchr(path, '\\')) ? strrchr(path, '\\') + 1 : path)

#else
    #define BASE_NAME(path) ((strrchr(path, '/')) ? strrchr(path, '/') + 1 : path)

#endif 

#define DEBUG_OUTPUT(content) \
    do { \
        const char * file_name = BASE_NAME(__FILE__); \
        cout << boolalpha; \
        cout << "[" << __LINE__ << ":" << file_name << "] " \
        << content << "\n"; \
    } while(0)

#define internal_assert(condition, message) \
    do { \
        if (!(condition)) { \
            cerr << "\n"; \
            cerr << "Assertion FAILED\n"; \
            cerr << "Condition: " << #condition << "\n"; \
            cerr << "Message: " << message << "\n"; \
            cerr << "File: " << __FILE__ << "\n"; \
            cerr << "Line: " << __LINE__ << "\n"; \
            abort(); \
        } \
    } while(0)

#endif // __ASSERT_HXX