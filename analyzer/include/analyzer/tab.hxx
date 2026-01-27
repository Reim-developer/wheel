#if !defined(TAB_HXX)
#define TAB_HXX
#include <string>
#include <string_view>
#include <vector>
#include "analyzer/error.hxx"

using std::string;
using std::vector;
using std::string_view;
using analyzer::Diagnostic;

namespace analyzer {
    enum class IndentType {
        SPACE,
        TAB,
        NEWLINE
    };

    struct IndentResult {
        vector<IndentType> indents;
        vector<Diagnostic> diagnostics;
    };

    class TabAnalyzer {
        public:
            static IndentResult analyzer(
                const char* start,
                const char* end,
                size_t line, 
                string_view source_line,
                string_view file_name
            );
    };
}

#endif // TAB_HXX