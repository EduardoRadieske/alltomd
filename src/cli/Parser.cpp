#include <alltomd/cli/Options.h>
#include <CLI/CLI.hpp>

namespace alltomd {

    namespace cli {

        ConversionOptions parseCommandLine(int argc, char* argv[]) {
            CLI::App app{"alltomd - Convert structured data to Markdown format"};
            
            ConversionOptions opts;
            
            app.add_option("input_file", opts.input_file, "Input file to convert")
                ->check(CLI::ExistingFile);

            try {
                app.parse(argc, argv);
            } catch (const CLI::ParseError& e) {
                exit(e.get_exit_code());
            }
            
            return opts;
        }

    } // namespace cli

} // namespace alltomd