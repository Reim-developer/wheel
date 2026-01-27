#if !defined(TAB_HXX)
#define TAB_HXX
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace analyzer {
    enum class IndentType {
        SPACE,
        TAB,
        NEWLINE
    };

    struct IndentError {
        enum Type {
            LEADING_WHITE_SPACE_ERROR,
            LEADING_TAB_ERROR,
            MIXED_INDENTATION_ERROR,
        } error_type;

        string messsage;
        size_t line;
        size_t column;
    };

    struct IndentResult {
        vector<IndentType> indents;
        vector<IndentError> errors;
    };

    class TabAnalyzer {
        public:
            static IndentResult analyzer(
                const char* start,
                const char* end,
                size_t line
            );
    };
}

#endif // TAB_HXX