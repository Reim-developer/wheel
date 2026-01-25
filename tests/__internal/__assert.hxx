#ifndef __ASSERT_HXX
#define __ASSERT_HXX

#include <iostream>
#include <cstdlib>

using std::cerr;
using std::abort;

#if defined(_MSC_VER)
    #pragma warning(disable: 4806)
#endif 

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