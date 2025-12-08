#include "../header/grammar.h"
#include "../header/logger.h"

Grammar::Grammar()
{
    this->terminals = {};
    this->startSymbol = "";
    this->productions = {};
}

Grammar::Grammar(string startSymbol, set<string> terminals)
{
    this->startSymbol = startSymbol;
    this->terminals = terminals;
}

void Grammar::removeVariable(const string &A)
{
    variables.erase(A);
    productions.erase(A);

    for (auto &pair : productions)
    {
        auto &prodSet = pair.second;
        set<vector<string>> toRemove;
        set<vector<string>> toAdd;

        for (const auto &rhs : prodSet)
        {
            vector<string> newRhs;
            bool changed = false;

            for (const string &sym : rhs)
            {
                if (sym == A)
                {
                    changed = true;
                }
                else
                {
                    newRhs.push_back(sym);
                }
            }

            if (changed)
            {
                toRemove.insert(rhs);
                if (!newRhs.empty())
                {
                    toAdd.insert(newRhs);
                }
            }
        }

        for (const auto &r : toRemove)
            prodSet.erase(r);
        for (const auto &r : toAdd)
            prodSet.insert(r);
    }
}

void Grammar::addProduction(string A, vector<string> rhs)
{
    productions[A].insert(rhs);
    this->addVariable(A);
}

void Grammar::addVariable(string A)
{
    variables.insert(A);
}

void Grammar::removeProduction(string A, vector<string> rhs)
{
    productions[A].erase(rhs);
    if (productions[A].empty())
    {
        productions.erase(A);
    }
}

void Grammar::clearProductions(string A)
{
    productions.erase(A);
}

set<vector<string>> Grammar::getProductions(string A)
{
    auto it = productions.find(A);
    if (it != productions.end())
    {
        return it->second;
    }
    return set<vector<string>>();
}

void Grammar::print(ostream &out)
{
    bool aux = true;
    out << "Gramática atual... \n";

    out << "Terminais: <";
    for (auto t : terminals)
    {
        if (!aux)
        {
            out << ", ";
        }
        aux = false;
        out << t;
    }
    out << ">\n";

    auto startGrammar = productions.find(startSymbol);
    if (startGrammar != productions.end())
    {
        out << startGrammar->first << " -> ";
        aux = true;
        for (auto t : startGrammar->second)
        {
            if (!aux)
            {
                out << " | ";
            }
            aux = false;
            for (auto p : t)
            {
                out << p;
            }
        }
        out << "\n";
    }
    for (auto p : productions)
    {
        if (p.first == startSymbol)
        {
            continue;
        }
        out << p.first << " -> ";
        aux = true;
        for (auto ps : p.second)
        {
            if (!aux)
            {
                out << " | ";
            }
            aux = false;
            for (auto pss : ps)
            {
                out << pss;
            }
        }
        out << "\n";
    }
    out << "\n";
}

Grammar Grammar::clone() const
{
    Grammar g2;
    g2.terminals = this->terminals;
    g2.startSymbol = this->startSymbol;
    g2.productions = this->productions;
    g2.variables = this->variables;
    return g2;
}

bool Grammar::isVariable(const string &symbol) const
{
    return variables.count(symbol) > 0;
}

/* SEÇÃO DE GREIBACH */

vector<string> getSortedVariables(const unordered_map<string, int> &map)
{
    vector<string> sortedVariables(map.size());
    for (auto [val, idx] : map)
    {
        sortedVariables[idx - 1] = val;
    }

    return sortedVariables;
}

void Grammar::renameVariables()
{
    this->next_order = 2;
    auto start = this->getStartSymbol();
    this->order[start] = 1;

    for (auto v : this->getVariables())
    {
        if (v != start)
        {
            this->order[v] = this->next_order++;
        }
    }

    Logger::log() << "Numerando em ordem as variáveis da gramática. \n";
    auto sorted = getSortedVariables(this->order);

    for (auto v : sorted)
    {
        Logger::log() << v << ": " << order[v] << "\n";
    }
}

bool Grammar::respectGreibachOrder()
{
    for (auto k : this->variables)
    {
        for (vector<string> p : this->getProductions(k))
        {
            string first_var = p.front();

            if (this->isVariable(first_var))
            {
                if (order[first_var] <= order[k])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void Grammar::removeLeftRecursionGreibach(string lhs)
{
    Logger::log() << "Faremos a remoção das regras com recursão à esquerda em " << lhs << ".\n";
    set<vector<string>> productionsA = this->getProductions(lhs);
    vector<vector<string>> recursive;
    vector<vector<string>> non_recursive;
    string new_lhs = lhs + "'";

    // Divide as regras entre recursivas e não recursivas
    for (auto &prod : productionsA)
    {
        auto symbol = prod.front();
        if (symbol == lhs)
        {
            vector<string> beta(prod.begin() + 1, prod.end());
            recursive.push_back(beta);
        }
        else
        {
            non_recursive.push_back(prod);
        }
    }

    if (!recursive.empty())
    {
        // Remove todas as regras pra depois adicionar novamente, evitando mexer
        this->clearProductions(lhs);
        this->addVariable(new_lhs);
        this->order[new_lhs] = this->next_order++;
        this->variables.insert(new_lhs);

        Logger::log() << "Nova variável introduzida: " << new_lhs << "\n";

        // Criar agora as regras recursivas
        for (auto beta : recursive)
        {
            this->addProduction(new_lhs, beta); // A' -> B
            vector<string> betaB = beta;
            betaB.push_back(new_lhs);
            this->addProduction(new_lhs, betaB); // A' -> B | BA'
        }

        // Adicionar as regras não recursivas
        for (auto og : non_recursive)
        {
            this->addProduction(lhs, og); // A -> a

            vector<string> ogRec = og;
            ogRec.push_back(new_lhs);
            this->addProduction(lhs, ogRec); // A -> a | aA'
        }

        Logger::log() << "Atualizada numeração das variáveis da gramática. \n";

        auto sorted = getSortedVariables(order);
        for (auto v : sorted)
        {
            Logger::log() << v << ": " << order[v] << "\n";
        }
    }

    this->print(Logger::log());
}

void Grammar::applyRuleOrderConstraint()
{
    this->print(Logger::log());

    while (!this->respectGreibachOrder())
    {
        Logger::log() << "======================================================\n";
        Logger::log() << "Gramática ainda não está na Forma Normal de Greibach. \n";
        // Analisaremos cada regra de cada variável para ver se está no padrão.

        auto sorted = getSortedVariables(order);

        for (auto k : sorted)
        {
            for (auto p : this->getProductions(k))
            {
                string j = p.front();
                // Se começa com terminal ou se a regra Ak -> Aj tem j > k,
                // já está seguindo a ordem.
                if (!this->isVariable(j) || order[j] > order[k])
                {
                    continue;
                }
                else if (order[j] < order[k])
                {
                    this->replace(k, j);
                }
                else
                { // Aplica remoção de recursão à esquerda
                    this->removeLeftRecursionGreibach(k);
                }
            }
        }
    }
}

void Grammar::replace(string lhs, string vRhs)
{
    Logger::log() << "Faremos a substituição de " << vRhs << " em " << lhs << ".\n";
    set<vector<string>> to_remove;
    set<vector<string>> to_add;

    auto current_productions = this->getProductions(lhs);
    auto replacer_productions = this->getProductions(vRhs);

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
        this->removeProduction(lhs, prod);
    for (const auto &prod : to_add)
        this->addProduction(lhs, prod);

    this->print(Logger::log());
}

// TODO: adicionar logs aqui tbm
void Grammar::replaceBackwards()
{
    auto variables = getSortedVariables(order);

    for (auto v = variables.rbegin(); v != variables.rend(); v++)
    {
        for (auto p : this->getProductions(*v))
        {
            string j = p.front();
            string k = *v;

            if (!this->isVariable(j))
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

void Grammar::toGreibachNormalForm()
{
    Logger::log() << "Iniciando a normalização para Forma Normal de Greibach!\n";
    this->renameVariables();
    this->applyRuleOrderConstraint();
    this->replaceBackwards();
    // Step 4 não parece mais necessário agora...
}