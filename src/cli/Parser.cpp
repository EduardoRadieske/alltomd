#include <alltomd/cli/Options.h>
#include <CLI/CLI.hpp>

namespace alltomd {

    namespace cli {

        ConversionOptions parseCommandLine(int argc, char* argv[]) {
            CLI::App app{"alltomd - Convert structured data to Markdown format"};
            
            ConversionOptions opts;
            
            app.add_option("input_file", opts.input_file, "Input file to convert")
                ->check(CLI::ExistingFile);
            
            app.add_option("-o,--output", opts.output_file, "Output Markdown file");
            
            app.add_option("-f,--format", opts.format, "Output format: table, list, code, auto")
                ->check(CLI::IsMember({"table", "list", "code", "auto"}))
                ->default_str("auto");
            
            app.add_option("-d,--max-depth", opts.max_depth, "Maximum nesting depth (-1 = unlimited)")
                ->check(CLI::Range(-1, 100))
                ->default_val(-1);
            
            app.add_flag("--no-null", opts.show_null, "Hide null values")
                ->expected(false);
            
            // Metadata
            app.add_option("-t,--title", opts.title, "Report title");
            app.add_option("-a,--author", opts.author, "Report author");
            
            // Preview
            app.add_flag("-p,--preview", opts.preview_mode, "Preview mode (shows only part of the data)");
            app.add_option("-l,--lines", opts.preview_lines, "Number of lines in preview mode (default: 20)")
                ->check(CLI::PositiveNumber);

            
            try {
                app.parse(argc, argv);
            } catch (const CLI::ParseError& e) {
                exit(e.get_exit_code());
            }
            
            return opts;
        }

    } // namespace cli

} // namespace alltomd