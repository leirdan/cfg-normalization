#include "../header/greibachNormalizer.h"
#include <hash_map>

Grammar *GreibachNormalizer::renameVariables(const Grammar *g)
{
    Grammar original_grammar = g->clone();
    Grammar *new_grammar = new Grammar("V1", original_grammar.getTerminals());

    // Criando nova ordem de variáveis

    map<string, string> variables_map; // Map para relacionar as variáveis atuais com as novas
    int idx = 2;                       // 1 é reservado para S
    auto old_variables = original_grammar.getVariables();

    // Primeiro converte as variáveis
    for (auto v = old_variables.begin(); v != old_variables.end(); ++v)
    {
        auto var = *v;
        if (var == "S")
        {
            variables_map.insert({var, "V1"});
            new_grammar->addVariable("V1");
        }
        else
        {
            variables_map.insert({var, "V" + to_string(idx)});
            new_grammar->addVariable(variables_map[var]);
            idx++;
        }
    }

    // Conversão das produções para a específica.
    for (auto v = old_variables.begin(); v != old_variables.end(); ++v)
    {
        set<vector<string>> productions = original_grammar.getProductions(*v);

        // Para cada produção, mapeia para as variáveis do map e, ao final, adiciona na nova gramática.
        for (auto p : productions)
        {
            vector<string> new_rhs;
            for (auto rule : p)
            {
                for (auto it = rule.begin(); it != rule.end(); it++)
                {
                    string symbol;
                    symbol.push_back(*it);
                    if (original_grammar.isVariable(symbol))
                    {
                        new_rhs.push_back(variables_map[symbol]);
                    }
                    else
                    {
                        new_rhs.push_back(symbol);
                    }
                }
            }

            new_grammar->addProduction(variables_map[*v], new_rhs);
        }
        // cout << new_gramar;
    }
    return new_grammar;
}

Grammar *GreibachNormalizer::normalize(const Grammar &g)
{
    Grammar *grammar = this->renameVariables(&g);
    return grammar;
}