#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, ".UTF8");
    system("chcp 65001 > nul");

    cout << "Iniciando programa..." << endl;

    if (argc > 1) {
        ifstream arq(argv[1], ios::binary);

        if (!arq.is_open()) {
            cout << "Erro ao abrir arquivo" << endl;
            return 1;
        }

        string conteudo(
            (istreambuf_iterator<char>(arq)),
            istreambuf_iterator<char>()
        );

        auto json = json::parse(conteudo);

        for (auto it : json)
        {
            cout << "value: " << it << endl;
        }

        cout << "\nO CONTEÚDO DO ARQUIVO É:\n" << conteudo << endl;
    } else {
        printf("Informe o arquivo. Ex.: ./program arquivo.txt\n");
    }

    return 0;
}