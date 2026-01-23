#include <iostream>
#include <string>
#include <tao/pegtl.hpp>

using std::cout;
using std::getline;
using std::cin;
using std_string = std::string;
using tao::pegtl::seq;
using tao::pegtl::one;
using tao::pegtl::string;
using tao::pegtl::parse;
using tao::pegtl::string_input;

struct grammar : seq<string <'h', 'e', 'l', 'l', 'o'>, one<' '>, string<'w', 'o', 'r', 'l', 'd'>>{};

int main() {
    std_string input;
    cout << "Input something" << "\n";
    getline(cin, input);
    
    if(parse<grammar>(string_input<>(input, "input"))) {
        std::cout << "Ok" << "\n";
    } else {
        std::cout << "fail" << "\n";
    }
    
    return 0;
}