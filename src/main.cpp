#include <fstream>
#include <iostream>
#include "../header/grammar.h"

int main() {
    ofstream file("../grammar.txt");

    Grammar g("S", { "a", "b" });

    g.addProduction("S", {"a", "S", "b"});
    g.addProduction("S", {"a", "b"});
    g.addProduction("S", {"b"});
    g.addProduction("A", {"b"});

    cout << "Gramática original:\n";
    g.print(cout);

    cout << "\nProduções de \"S\" obtidas por getProductions:\n";
    auto prodsS = g.getProductions("S");
    for (const auto& rhs : prodsS) {
        cout << "  S ->";
        for (const auto& sym : rhs) {
            cout << " \"" << sym << "\"";
        }
        cout << endl;
    }

    cout << "\nProduções de \"X\" (não existe na gramática):\n";
    auto prodsX = g.getProductions("X");
    cout << "  quantidade de produções de X: " << prodsX.size() << endl;

    cout << "\nGramática original (para conferir que não mudou):\n";
    g.print(file);
    return 0;
}
