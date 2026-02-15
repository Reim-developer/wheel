#ifndef __ASSERT_HXX
#define __ASSERT_HXX

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <random>
#include <format> // IWYU pragma: keep
#include <wheel_lexer/token.hxx>

using std::cerr;
using std::cout;
using std::boolalpha;
using std::abort;
using std::string;
using std::random_device;
using std::mt19937;
using std::uniform_int_distribution;
using wheel_lexer::TokenKind;

#if defined(_MSC_VER)
    #pragma warning(disable: 4806)
#endif 

#if defined(_WIN32)
    #define BASE_NAME(path) ((strrchr(path, '\\')) ? strrchr(path, '\\') + 1 : path)

#else
    #define BASE_NAME(path) ((strrchr(path, '/')) ? strrchr(path, '/') + 1 : path)

#endif 

#define NO_TEST(func_name) void func_name() {}

#define TOKEN(kind, arena)  arena.allocate<Token> ( \
        kind, \
        "dummy_token", 1, 1 \
     )

#define DEBUG_OUTPUT(content) \
    do { \
        const char * file_name = BASE_NAME(__FILE__); \
        cout << boolalpha; \
        cout << "[" << __LINE__ << ":" << file_name << "] " \
        << content << "\n"; \
    } while(0)

#define assert_eq(x, y) \
    do { \
        if ((x != y)) { \
            cerr << "\n"; \
            cerr << std::format("{} is not equal of: {}", #x, #y); \
            abort(); \
        } else { \
            cout << std::format("{} is equal of: {}\n", #x, #y); \
        }\
    } while(0);

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
        } else { \
            const auto fmt = std::format("[OK] [{}:{}]", BASE_NAME(__FILE__),  __LINE__); \
            std::cout << fmt << std::format(" {} is {}\n", #condition, condition); \
        } \
    } while(0)

inline string random_str(size_t length) {
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

inline string rand_source_digits(
        bool exponent   = false, bool negative = false,
        bool rand_float = false) {
    random_device                    random;
    mt19937                          generator(random());
    uniform_int_distribution<size_t> distribution(0, 9);

    size_t len = distribution(generator) + 1;
    string results;

    for(size_t index = 0; index < len; ++index) {
        results.push_back('0' + distribution(generator));
    }

    if(rand_float) {
        results.push_back('.');

        size_t decimal_len = distribution(generator) % 10 + 1;
        for(size_t index = 0; index < decimal_len; ++index) {
            results.push_back('0' + distribution(generator));
        }
    }

    if (exponent) {
        results.push_back('e');
        if (negative) {
            results.push_back('-');
        }

        size_t exponent_len = distribution(generator) % 5 + 1;
        for (size_t index = 0; index < exponent_len; ++index) {
            results.push_back('0' + distribution(generator));
        }
    }

    return results;
}

#define TEST(func_name) void func_name() { \
    std::cout << std::format("[TEST] {}", #func_name) << "\n";

#define DONE }
#define RUN(func) func();

#define TEST_MAIN int main() { 
#define END_MAIN return 0; }

#endif // __ASSERT_HXX