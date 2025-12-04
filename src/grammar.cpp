#include "../header/grammar.h"

Grammar::Grammar() {
  this->terminals = {};
  this->startSymbol = "";
  this->productions = {};
}

Grammar::Grammar(string startSymbol, set<string> terminals) {
  this->startSymbol = startSymbol;
  this->terminals = terminals;
}

void Grammar::addProduction(string A, vector<string> rhs){
  productions[A].insert(rhs);
}

void Grammar::removeProduction(string A, vector<string> rhs){
  productions[A].erase(rhs);
}

set<vector<string>> Grammar::getProductions(string A){
  auto it = productions.find(A);
  if (it != productions.end()) {
    return it->second;
  }
  return set<vector<string>>();
}

void Grammar::print(ostream &out){
  out << "Terminais: <";
  bool first = true;
  for (auto t : terminals) {
    if (!first) {
      out << ", ";
    }
    first = false;
    out << t;
  }
  out << ">\n\n";

  first = true;
  out << "Produções:\n";
  for (auto p : productions) {
    out << p.first << " -> ";
    for (auto ps : p.second) {
      if (!first) {
        out << " | ";
      }
      first = false;
      for (auto pss : ps) {
        out << pss;
      }
    }
    out << "\n";
  }

  out << "\nInicial: " << this->startSymbol << "\n";
}

Grammar Grammar::clone() const {
  Grammar g2;
  g2.terminals = this->terminals;
  g2.startSymbol = this->startSymbol;
  g2.productions = this->productions;
  return g2;
}