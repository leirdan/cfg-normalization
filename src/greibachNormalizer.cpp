#include "../header/greibachNormalizer.h"
#include <hash_map>
#include <algorithm>

void GreibachNormalizer::renameVariables()
{
    int index = 2;
    for (auto v : this->grammar.getVariables())
    {
        if (v == "S")
        {
            this->order.insert({"S", 1});
        }
        else
        {
            this->order.insert({v, index});
            index++;
        }
    }
}

bool GreibachNormalizer::hasOrder()
{
    for (auto k : this->grammar.getVariables())
    {
        for (vector<string> p : this->grammar.getProductions(k))
        {
            string first_var = p.front();

            if (this->grammar.isVariable(first_var))
            {
                if (order[first_var] < order[k])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void GreibachNormalizer::applyRuleOrderConstraint()
{
    while (!this->hasOrder())
    {
        // Analisaremos cada regra de cada variável para ver se está no padrão.
        for (auto k : this->grammar.getVariables())
        {
            for (auto p : this->grammar.getProductions(k))
            {
                string j = p.front();
                // Se começa com terminal ou se a regra Ak -> Aj tem j > k,
                // já está seguindo a ordem.
                if (!this->grammar.isVariable(j) || order[j] > order[k])
                {
                    continue;
                }
                else if (order[j] < order[k])
                {
                    this->replace(k, j);
                }
                else
                { // Aplica remoção de recursão à esquerda
                }
            }
        }
    }
}

void GreibachNormalizer::replace(string lhs, string vRhs)
{
    set<vector<string>> to_remove;
    set<vector<string>> to_add;

    auto current_productions = this->grammar.getProductions(lhs);
    auto replacer_productions = this->grammar.getProductions(vRhs);

    for (const auto &p : current_productions)
    {
        if (!p.empty() && p.front() == vRhs)
        {
            to_remove.insert(p);

            // extrai o resto da regra para além da 1a variábel
            vector<string> beta;
            if (p.size() > 1)
            {
                beta.assign(p.begin() + 1, p.end());
            }

            // para cada regra de A_j, adicionamos em A_k seguido do beta
            for (const auto &replacer_rhs : replacer_productions)
            {
                vector<string> new_rhs = replacer_rhs;
                new_rhs.insert(new_rhs.end(), beta.begin(), beta.end());
                to_add.insert(new_rhs);
            }
        }
    }

    // aplica as mudanças
    for (const auto &prod : to_remove)
        this->grammar.removeProduction(lhs, prod);
    for (const auto &prod : to_add)
        this->grammar.addProduction(lhs, prod);
}

void GreibachNormalizer::replaceBackwards()
{
    auto variables = this->grammar.getVariables();
    for (auto v = variables.rbegin(); v != variables.rend(); v++)
    {
        for (auto p : this->grammar.getProductions(*v))
        {
            string j = p.front();
            string k = *v;

            if (!this->grammar.isVariable(j))
            {
                continue;
            }
            else if (order[j] > order[k])
            {
                this->replace(k, j); // Forçar com que todas as regras de k que iniciam com j iniciem com terminais
            }
        }
    }
}

Grammar *GreibachNormalizer::normalize()
{
    this->renameVariables();          // OK
    this->applyRuleOrderConstraint(); // OK
    this->replaceBackwards();

    return &grammar;
}