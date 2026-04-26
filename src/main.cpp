#include <iostream>
#include <fstream>
#include <string>
#include <alltomd/converter/JsonConverter.h>
#include <alltomd/cli/Options.h>

using namespace std;

using json = nlohmann::json;
namespace cli = alltomd::cli;
namespace converter = alltomd::converter;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, ".UTF8");
    system("chcp 65001 > nul");

    auto opts = cli::parseCommandLine(argc, argv);
    
    if (!opts.hasInput()) {
        cerr << "Error: No input file specified." << endl;
        cerr << "Usage: ./alltomd arquivo.json" << endl;
        return 1;
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
        
        cout << "Parsed data: " << endl;
        cout << jsonConverter.format(data) << endl;
    } catch (const exception& e) {
        cerr << "Error parsing: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}