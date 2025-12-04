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
        string getStartSymbol(){return startSymbol;}
        void setStartSymbol(string A){this->startSymbol = A;}
        void print(ostream &out);
        Grammar clone() const;
};

#endif