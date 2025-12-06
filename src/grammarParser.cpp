#include "../header/grammarParser.h"

Grammar GrammarParser::parserFromFile(string filename) {
    Grammar grammar;
    vector<string> v;
    ifstream file;
    file.open(filename);

    if (!file.is_open()) {
        cout << "\033[0m" << "Erro: não foi possível abrir o arquivo!" << "\033[31m" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string ladoEsquerdo, seta;
        v.clear();

        ss >> ladoEsquerdo;
        ss >> seta;

        if (seta == "->") {
            char c;
            while (ss.get(c)) {
                if (c != ' ') {
                    v.push_back(string(1, c));
                }
            }
            grammar.addProduction(ladoEsquerdo, v);
        }
    }
    return grammar;
}