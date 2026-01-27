#include "analyzer/tab.hxx"
#include <format>

using analyzer::IndentResult;
using analyzer::TabAnalyzer;
using analyzer::DiagnosticCategory;
using std::format;

IndentResult TabAnalyzer::analyzer(
            const char *start, const char* end, 
            size_t line, string_view source_line,
            string_view file_name) {
    IndentResult result;
    bool has_space      = false;
    bool has_tab        = false;
    size_t space_count  = 0;
    size_t tab_count    = 0;

    for (const char *ptr = start; ptr < end; ++ptr) {
        switch (*ptr) {
            case ' ':
                result.indents.push_back(IndentType::SPACE);
                space_count++;
                has_space = true;
                break;

            case '\t':
                result.indents.push_back(IndentType::TAB);
                tab_count++;
                has_tab = true;
                break;

            case '\n':
                result.indents.push_back(IndentType::NEWLINE);
                break;
        }
    }

    if (has_space && has_tab) {
        auto error_details = format(
            "Mixed indentation is not allowed. Found "
            "{} space(s) and {} tab(s)",
            space_count, tab_count
        );

        auto quick_fix = format(
            "[+] Quick Fix: Remove {} space(s) and {} "
            "tab(s) at the beginning of the line",
            space_count, tab_count
        );

        result.diagnostics.push_back(DiagnosticBuilder::new_error(
            file_name,
            line,
            1,
            error_details,
            source_line,
            0,
            space_count + tab_count,
            quick_fix,
            DiagnosticCategory::INDENTATION
        ));
    }

    if (has_space && !has_tab) {
        auto errror_details = format(
            "Leading space is not allowed. Found {} space(s)",
            space_count
        );
        auto quick_fix = format(
            "[+] Quick Fix: Remove {} space(s) at the beginning of the line",
            space_count
        );

        result.diagnostics.push_back(DiagnosticBuilder::new_error(
            file_name,
            line, 1, errror_details,
            source_line, 0,
            space_count,
            quick_fix,
            DiagnosticCategory::INDENTATION
        ));
    }

    if (has_tab && !has_space) {
        auto error_details = format(
            "Leading tab is not allowed outside indent blocks. "
            "Found {} tab(s)",
            tab_count
        );

        auto quick_fix = format(
            "[+] Quick Fix: Remove {} tab(s) at the "
            "beginning of the line",
            tab_count
        );

        result.diagnostics.push_back(DiagnosticBuilder::new_error(
            file_name,
            line,
            1,
            error_details,
            source_line,
            0, 
            tab_count,
            quick_fix,
            DiagnosticCategory::INDENTATION
        ));
    }

    return result;
}