#include "wheel_parser/functions/contracts_declaration.hxx"
#include <__internal/__assert.hxx>

#include <wheel_lexer/lexer.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_parser/parser.hxx>
#include <wheel_parser/functions/functions_declaration.hxx>

using wheel_lexer::Lexer;
using wheel_memory::Arena;
using wheel_parser::WheelParser;
using wheel_parser::ast::StringInterner;
using wheel_parser::functions::ParserFuncContract;
using wheel_parser::functions::ParserState;
using wheel_parser::functions::parse_function_declaration;


TEST(test_parse_normal_function_definition)
    using Contract = ParserFuncContract;

    Arena arena;
    Lexer lexer("function x(x: int, y: Something.) -> void:");
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);

    auto parse_state =  ParserState {
        .current_token = parser.m_current_token,
        .error_list = parser.m_errors
    };
    auto result = parse_function_declaration(Contract {
        .lexer      = lexer,
        .arena      = arena,
        .interner   = interner,
        .state      = parse_state
    });

    for (auto index = 0; index  < parse_state.error_list.size();++index) {
        DEBUG_OUTPUT(FORMAT("{}", parse_state.error_list.data()[index].message));
    }
DONE


TEST_MAIN
    RUN(test_parse_normal_function_definition)
END_MAIN
