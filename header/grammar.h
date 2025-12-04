#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Grammar{
private:
    set<string> terminals;
    string startSymbol;
    map<string, set<vector<string>>> productions;
public:
    Grammar();
    Grammar(string startSymbol, set<string> terminals);

    void addProduction(string A, vector<string> rhs);
    void removeProduction(string A, vector<string> rhs);
    set<vector<string>> getProductions(string A);
    void print(ostream &out);
    Grammar clone() const; // Modified signature to be a const member function returning a copy
};

#endif