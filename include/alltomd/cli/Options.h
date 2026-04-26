#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>

namespace alltomd {

    namespace cli {

        struct ConversionOptions {
            std::string input_file;
            bool stdin_mode{false};
            std::string output_file;

            std::string format{"auto"};          // table, list, code, auto
            std::string flatten_separator;       // "" = desabilitado
            int max_depth{-1};                   // -1 = ilimitado
            bool show_null{true};                // --no-null = false

            std::string title;
            std::string author;
            
            // Preview
            bool preview_mode{false};            // --preview
            int preview_lines{20};               // --lines (padrão 20)
            
            ConversionOptions() = default;
            
            bool hasInput() const {
                return stdin_mode || !input_file.empty();
            }
        };

        int parseCommandLine(int argc, char* argv[], ConversionOptions& opts);

    } // namespace cli

} // namespace alltomd

#endif // OPTIONS_H