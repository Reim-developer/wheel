#include "analyzer/tab.hxx"

using analyzer::IndentResult;
using analyzer::TabAnalyzer;

IndentResult TabAnalyzer::analyzer(
            const char *start, const char* end, 
            size_t line) {
    IndentResult result;
    bool has_space = false;
    bool has_tab   = false;

    for (const char *ptr = start; ptr < end; ++ptr) {
        switch (*ptr) {
            case ' ':
                result.indents.push_back(IndentType::SPACE);
                has_space = true;
                break;

            case '\t':
                result.indents.push_back(IndentType::TAB);
                has_tab = true;
                break;

            case '\n':
                result.indents.push_back(IndentType::NEWLINE);
                break;
        }

        if (has_space && has_tab) {
            result.errors.push_back({
                IndentError::MIXED_INDENTATION_ERROR,
                "Mixed indentation is not allowed. Just use TAB for indentation",
                line, 1
            });
        }

        if (has_space && !has_tab) {
            result.errors.push_back({
                IndentError::LEADING_WHITE_SPACE_ERROR,
                "Leading whitespace is not allowed",
                line, 1
            });
        }

        if (has_tab && !has_space) {
            result.errors.push_back({
                IndentError::LEADING_TAB_ERROR,
                "Leading tab is not allowed outside indent blocks",
                line, 1
            });
        }
    }

    return result;
}