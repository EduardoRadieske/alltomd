#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>

namespace alltomd {

    namespace cli {

        struct ConversionOptions {
            std::string input_file;
            bool stdin_mode{false};
            std::string output_file;
            
            ConversionOptions() = default;
            
            bool hasInput() const {
                return stdin_mode || !input_file.empty();
            }
        };

        ConversionOptions parseCommandLine(int argc, char* argv[]);

    } // namespace cli

} // namespace alltomd

#endif // OPTIONS_H