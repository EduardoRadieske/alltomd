#include <alltomd/cli/Options.h>
#include <CLI/CLI.hpp>

namespace alltomd {

    namespace cli {

        int parseCommandLine(int argc, char* argv[], ConversionOptions& opts) {
            CLI::App app{"alltomd - Convert structured data to Markdown format"};
            
            app.add_option("input_file", opts.input_file, "Input file to convert")
                ->check(CLI::ExistingFile)
                ->required(false);
            
            app.add_option("-o,--output", opts.output_file, "Output Markdown file")
                ->description("Save output to file instead of stdout");
            
            app.add_option("-f,--format", opts.format, "Output format: table, list, code, auto")
                ->check(CLI::IsMember({"table", "list", "code", "auto"}))
                ->description("table, list, code, auto")
                ->default_str("auto");
            
            app.add_option("-d,--max-depth", opts.max_depth, "Maximum nesting depth (-1 = unlimited)")
                ->check(CLI::Range(-1, 100))
                ->description("-1 = unlimited")
                ->default_val(-1);
            
            app.add_flag("--no-null", opts.show_null, "Hide null values")
                ->description("Don't show null values in output")
                ->expected(false);
            
            // Metadata
            app.add_option("-t,--title", opts.title, "Report title")
                ->description("Title of the Markdown report");
            app.add_option("-a,--author", opts.author, "Report author")
                ->description("Author of the Markdown report");
            
            // Preview
            app.add_flag("-p,--preview", opts.preview_mode, "Preview mode (shows only part of the data)")
                ->description("Show only part of the data");
            app.add_option("-l,--lines", opts.preview_lines, "Number of lines in preview mode (default: 20)")
                ->description("Only works with --preview")
                ->check(CLI::PositiveNumber);

            app.set_version_flag("--version", "alltomd v1.0.0");
            
            CLI11_PARSE(app, argc, argv);
        }

    } // namespace cli

} // namespace alltomd