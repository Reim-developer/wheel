#include <filesystem>
#include <fstream>
#include <iterator>
#include <iostream>
#include <string>
#include <system_error>

#include <wheel_lexer/lexer.hxx>
#include <wheel_interpreter/interpreter.hxx>
#include <wheel_memory/allocator.hxx>
#include <wheel_parser/parser.hxx>
#include <wheel_semantic/semantic.hxx>

using std::filesystem::path;
using wheel_lexer::Lexer;
using wheel_interpreter::WheelInterpreter;
using wheel_memory::Arena;
using wheel_parser::WheelParser;
using wheel_parser::ast::StringInterner;
using wheel_semantic::SemanticAnalyzer;

int print_usage(const path &exec_name) {
    std::cerr << "Usage: "
            << exec_name.filename().string()
            << " <file_name>\n";
    return 1;
}

bool read_source_file(const path &source_path, std::string &source_content) {
    std::error_code filesystem_error;
    if (!std::filesystem::exists(source_path, filesystem_error)) {
        std::cerr << "Source file not found: " << source_path << '\n';
        return false;
    }

    if (!std::filesystem::is_regular_file(source_path, filesystem_error)) {
        std::cerr << "Source path is not a regular file: " << source_path << '\n';
        return false;
    }

    std::ifstream source_stream(source_path, std::ios::binary);
    if (!source_stream.is_open()) {
        std::cerr << "Cannot open source file: " << source_path << '\n';
        return false;
    }

    source_content.assign(
        std::istreambuf_iterator<char>(source_stream),
        std::istreambuf_iterator<char>()
    );

    return true;
}

void print_parse_diagnostics(const WheelParser &parser, const path &source_path) {
    const auto diagnostics = parser.collect_diagnostics(source_path.string());
    for (size_t index = 0; index < diagnostics.size(); ++index) {
        const auto &diagnostic = diagnostics.data()[index];
        std::cerr   << diagnostic.full_message << '\n'
                    << diagnostic.source_line << '\n'
                    << diagnostic.marker << '\n';
    }
}

void print_runtime_diagnostics(
    const WheelInterpreter &interpreter,
    Lexer &lexer,
    const path &source_path
) {
    const auto *errors = interpreter.errors_data();
    for (size_t index = 0; index < interpreter.error_count(); ++index) {
        const auto &error = errors[index];
        std::cerr << "[R" << static_cast<uint16_t>(error.code) << "] ";

        if (error.token == nullptr) {
            std::cerr << "[" << source_path.string() << "] " << error.message << '\n';
            continue;
        }

        const auto location = lexer.get_source_location(*error.token);
        const auto source_line = lexer.get_source_line(location.offset);
        size_t marker_size = static_cast<size_t>(error.token->end - error.token->start);
        if (marker_size == 0) {
            marker_size = 1;
        }

        std::cerr << "[" << source_path.string() << ":"
                  << location.line << ":" << location.column << "] "
                  << error.message << '\n'
                  << source_line << '\n'
                  << std::string(static_cast<size_t>(location.column - 1), ' ')
                  << std::string(marker_size, '^') << '\n';
    }
}

void print_semantic_diagnostics(
    const SemanticAnalyzer &semantic,
    Lexer &lexer,
    const path &source_path
) {
    const auto *errors = semantic.errors_data();
    for (size_t index = 0; index < semantic.error_count(); ++index) {
        const auto &error = errors[index];
        std::cerr << "[S" << static_cast<uint16_t>(error.code) << "] ";

        if (error.token == nullptr) {
            std::cerr << "[" << source_path.string() << "] " << error.message << '\n';
            continue;
        }

        const auto location = lexer.get_source_location(*error.token);
        const auto source_line = lexer.get_source_line(location.offset);
        size_t marker_size = static_cast<size_t>(error.token->end - error.token->start);
        if (marker_size == 0) {
            marker_size = 1;
        }

        std::cerr << "[" << source_path.string() << ":"
                  << location.line << ":" << location.column << "] "
                  << error.message << '\n'
                  << source_line << '\n'
                  << std::string(static_cast<size_t>(location.column - 1), ' ')
                  << std::string(marker_size, '^') << '\n';
    }
}

int run_parser_cli(const path &source_path) {
    std::string source_content;
    if (!read_source_file(source_path, source_content)) {
        return 1;
    }

    Arena arena;
    Lexer lexer(source_content);
    StringInterner interner;
    WheelParser parser(lexer, arena, interner);

    parser.parse();
    if (parser.error_count() > 0) {
        print_parse_diagnostics(parser, source_path);
        return 1;
    }

    SemanticAnalyzer semantic(arena, interner);
    if (!semantic.analyze(parser.statements_data(), parser.statement_count())) {
        print_semantic_diagnostics(semantic, lexer, source_path);
        return 1;
    }

    WheelInterpreter interpreter(arena);
    if (!interpreter.execute(semantic.program())) {
        print_runtime_diagnostics(interpreter, lexer, source_path);
        return 1;
    }

    return 0;
}

int main(int argc, char** argv) {
    const path exec_name = argv[0];

    if (argc != 2) {
        return print_usage(exec_name);
    }

    return run_parser_cli(path(argv[1]));
}
