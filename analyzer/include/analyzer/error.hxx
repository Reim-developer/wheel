#if !defined(ERROR_HXX)
#define ERROR_HXX

#include <string>
#include <string_view>
#include <format>

using std::string;
using std::string_view;
using std::format;

namespace analyzer {
    enum class DiagnosticCategory {
            GENERAL,
            INDENTATION,
            SYNTAX,
            SEMANTIC,
            TYPE,
            IMPORT,
            STYLE
       };

    struct Diagnostic {
        enum class Level {
            ERROR,
            WARNING,
            INFO
       };

       Level level;
       string full_message;
       string error_details;
       string quick_fix;
       string error_marker;
       size_t line;
       size_t column;
       string_view source_line;
       string_view file_name;
       size_t error_start_position;
       size_t error_length;
       DiagnosticCategory category;
    };

    struct DiagnosticBuilder {
        using D = DiagnosticCategory;

        static string category_to_str(DiagnosticCategory category) {
            
            switch (category) {
                case D::GENERAL:        return "general";
                case D::INDENTATION:    return "identation";
                case D::SYNTAX:         return "syntax";
                case D::SEMANTIC:       return "semantic";
                case D::TYPE:           return "type";
                case D::IMPORT:         return "import";
                case D::STYLE:          return "style";
                default:                return "general";
            }
        }

        static Diagnostic new_error(
            string_view file_name,
            size_t      line,
            size_t      column,
            string_view error_details,
            string_view source_line          = "",
            size_t      error_start_position = 0,
            size_t      error_length         = 0,
            string_view quick_fix            = "",
            D           category             = D::GENERAL
        ) {
            Diagnostic diag;
            diag.level          = Diagnostic::Level::ERROR;
            diag.error_details  = string(error_details);
            diag.quick_fix      = string(quick_fix);
            diag.category       = category;
            diag.line           = line;
            diag.column         = column;
            diag.source_line    = source_line;
            diag.file_name      = file_name;

            diag.error_start_position = error_start_position;
            diag.error_length         = error_length;

            diag.error_marker = string(error_length, '^');
            
            const auto err_file_name = file_name.empty() ? "unknown" : file_name.data();
            diag.full_message = format(
                "[{}:{}:{}] {}",
                line, column, err_file_name, error_details
            );

            return diag;
        }
    };

}

#endif // ERROR_HXX