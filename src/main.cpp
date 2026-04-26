#include <iostream>
#include <fstream>
#include <string>
#include <alltomd/converter/JsonConverter.h>
#include <alltomd/cli/Options.h>
#include <alltomd/writer/MarkdownWriter.h>

using namespace std;

using json = nlohmann::json;
namespace cli = alltomd::cli;
namespace converter = alltomd::converter;
namespace writer = alltomd::writer;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, ".UTF8");
    system("chcp 65001 > nul");

    cli::ConversionOptions opts;
    int result = cli::parseCommandLine(argc, argv, opts);

    if (!opts.hasInput()) {
        return 0;
    }
    
    cout << "Input file: " << opts.input_file << endl;
    
    ifstream arq(opts.input_file, ios::binary);
    if (!arq.is_open()) {
        cerr << "Error: Could not open file: " << opts.input_file << endl;
        return 1;
    }
    
    string conteudo(
        (istreambuf_iterator<char>(arq)),
        istreambuf_iterator<char>()
    );
    arq.close();

    converter::JsonConverter jsonConverter;
    if (!jsonConverter.canHandle(opts.input_file)) {
        cerr << "Error: File format not supported: " << opts.input_file << endl;
        return 1;
    }

    if (!jsonConverter.validate(conteudo)) {
        cerr << "Error: Invalid JSON format" << endl;
        return 1;
    }
    
    try {
        auto data = jsonConverter.parse(conteudo);
        
        writer::WriterOptions writerOpts;
        writerOpts.format = opts.format;
        writerOpts.maxDepth = opts.max_depth;
        writerOpts.showNull = opts.show_null;
        writerOpts.previewLines = opts.preview_mode ? opts.preview_lines : -1;
        writerOpts.title = opts.title;
        writerOpts.author = opts.author;
        
        // Convert to Markdown
        writer::MarkdownWriter mdWriter;
        string markdown = mdWriter.convert(data, writerOpts);
        
        // Output
        if (!opts.output_file.empty()) {
            ofstream out(opts.output_file);
            if (!out.is_open()) {
                cerr << "Error: Could not create output file: " << opts.output_file << endl;
                return 1;
            }
            out << markdown;
            cout << "Markdown saved to: " << opts.output_file << endl;
        } else {
            cout << markdown << endl;
        }
    } catch (const exception& e) {
        cerr << "Error parsing: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}