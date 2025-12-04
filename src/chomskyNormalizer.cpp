#include "../header/grammar.h"
#include "../header/chomskyNormalizer.h"
#include <string>

using namespace std;

ChomskyNormalizer::ChomskyNormalizer(const Grammar& g) {
  this->grammar = g;
}

Grammar ChomskyNormalizer::removeRecursionAtBeginning(){
    string S = (this->grammar).getStartSymbol();
    set<vector<string>> productionsS = (this->grammar).getProductions(S);
    
    for(auto& prod : productionsS){
        for(auto& symbol : prod){
          if(symbol == S){
            (this->grammar).setStartSymbol("S'");
            (this->grammar).addProduction("S'", {"S"});
            break; 
          }
        }
    }

    return (this->grammar);
}