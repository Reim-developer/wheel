#ifndef __ASSERT_HXX
#define __ASSERT_HXX

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <random>

using std::cerr;
using std::cout;
using std::boolalpha;
using std::abort;
using std::string;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;

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

string random_str(size_t length) {
    const char char_set[] = 
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "\n\t";
    
    random_device random;
    mt19937       genrator(random());
    uniform_int_distribution<size_t> distribution(0, sizeof(char_set) - 2);

    string result;
    result.reserve(length);

    for (size_t index = 0; index < length; ++index) {
        result += char_set[distribution(genrator)];
    }

    return result;
}

#endif // __ASSERT_HXX