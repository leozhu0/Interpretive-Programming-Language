#include "lib/token.h"
#include "lib/parser.h"
#include "lib/lexer.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <map>

int main() {
  
  Lexer lexer = Lexer();

  //run though each line
  std::vector<Token> tokens = lexer.lexer();

  std::vector<std::vector<Token>> multilineTokens;
 
 
  int index = 0; 
  //If END is on a new line, then stop at the line before, otherwise do the whole thing
  for(int i = 0; i < tokens[tokens.size() - 1 - ((tokens[tokens.size()-1].column == 1)?1:0)].line; i++){
    std::vector<Token> tempRow;
    
    while(index < (int)tokens.size() && tokens[index].line == i+1){
      tempRow.push_back(tokens[index]);
      index++; 
    }

    multilineTokens.push_back(tempRow);
  }


  for (const auto &line : multilineTokens)
  {
    try{
      Parser parser = Parser();

      parser.ParserFunc(line);
      std::cout << parser.toString() << std::endl << parser.calculate() << std::endl;
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }

  

  

	  
  return 0;
}
