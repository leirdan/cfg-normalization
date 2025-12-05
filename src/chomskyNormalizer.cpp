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

set<string> ChomskyNormalizer::findVoidableVariables(){
  set<string> voidableVariables;
  for(string variable : (this->grammar).getVariables()){
    for(vector<string> production : (this->grammar).getProductions(variable)){
      if(production.size() == 1){
        if(production[0] == "&"){
          voidableVariables.insert(variable);
          break;
        }
      }
    }
  }
  for(string v : voidableVariables){
    std::cout << v << " ";
  }
  cout<<endl;

  return voidableVariables;
}

Grammar ChomskyNormalizer::removeLambdaProductions(){
  set<string> voidableVariables = findVoidableVariables();
  set<string> variables = (this->grammar).getVariables();

  for(string variable : (this->grammar).getVariables()){
    for(vector<string> production : (this->grammar).getProductions(variable)){
      for(string rhs : production){

      }
    }
  }

}