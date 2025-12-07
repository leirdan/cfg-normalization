#ifndef GREIBACH_HPP
#define GREIBACH_HPP

#include "grammar.h"

class GreibachNormalizer
{
private:
    void eliminateIndirectLeftRecursion(Grammar *g);
    void eliminateDirectLeftRecursion(Grammar *g);
    Grammar *renameVariables(const Grammar *g);

public:
    GreibachNormalizer() {}
    Grammar *normalize(const Grammar &g);
};

#endif // !GREIBACH_HPP
