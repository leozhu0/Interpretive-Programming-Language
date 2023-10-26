#include "lib/token.h"
#include "lib/parser.cpp"
#include "lib/lexer.cpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <map>

int main() {
  Lexer lexer = Lexer();

  //run though each line
  std::vector<Token> tokens = lexer.lexer();

  for(int i = 0; i < (int)(tokens.size()); i++){
        std::cout << std::right << std::setw(4) << tokens.at(i).line << std::right << std::setw(5) << tokens.at(i).column << "  " << tokens.at(i).token << std::endl;
    }

  std::vector<std::vector<Token>> multilineTokens;
 
 
  
  //int prevLine = 1;
int index = 0; 
  for(int i = 0; i < tokens[tokens.size()-2].line; i++){
    std::vector<Token> tempRow;
    
    while(tokens[index].line == i+1){
      tempRow.push_back(tokens[index]);
      index++; 
    }

    multilineTokens.push_back(tempRow);
  }





for ( const auto &line : multilineTokens )
{
  Parser parser = Parser(line);
  std::cout << parser.toString() << std::endl << parser.calculate() << std::endl;
}


  for (const auto& pair : variables) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }


	  
  return 0;
}
