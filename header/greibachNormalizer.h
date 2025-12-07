#ifndef GREIBACH_HPP
#define GREIBACH_HPP

#include "grammar.h"
#include <unordered_map>

class GreibachNormalizer
{
private:
    Grammar grammar;
    unordered_map<string, int> order; // Para marcar a ordem de cada vértice

    bool hasOrder();
    void renameVariables();
    void replace(string lhs, string vRhs);
    void replaceBackwards();
    void applyRuleOrderConstraint();

public:
    GreibachNormalizer(Grammar g)
    {
        this->grammar = g.clone();
    }
    Grammar *normalize();
};

#endif // !GREIBACH_HPP
