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

  // Run though each line
  std::vector<Token> tokens = lexer.lexer();

  std::vector<std::vector<Token>> multilineTokens;

          
  //If END is on a new line, then stop at the line before, otherwise do the whole thing
  int lastIndex = tokens.size() - 2 - ((tokens[tokens.size()-1].column == 1)?1:0);
  int index = 0;
  while(index <= lastIndex){
    std::vector<Token> tempRow;
    
    int tempParensCount = 0;
    while(true){
      
      if(tokens[index].token == ")"){
        tempParensCount--;
      }

      if(tokens[index].token == "("){
        tempParensCount++;
      }

      tempRow.push_back(tokens[index]);
      index++;

      if(tempParensCount == 0 || index == lastIndex+2){
        break;
      }
    }

    
    if(index <= (int)(tokens.size() - 1) && tokens[index].token == ")"){
      std::cout << "Unexpected token at line " << tokens[index].line << " column " << tokens[index].column << ": " << tokens[index].token << std::endl;
      exit(2);
    }

    //PUT END AT THE END OF EACH LINE
    tempRow.push_back(Token{tempRow.back().line, tempRow.back().column+1,"END", END});


    multilineTokens.push_back(tempRow);
  }


  for (const auto &line : multilineTokens)
  {
    Parser parser = Parser();
    try{
      parser.ParserFunc(line);
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      return 1;
    }

    try {
      std::cout << parser.toString() << std::endl << parser.calculate() << std::endl;
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      if(((std::string)(e.what())).compare("Runtime error: division by zero.") == 0) {
        return 3;
      }
      if(((std::string)(e.what())).substr(0,33).compare("Runtime error: unknown identifier") == 0) {
        return 3;
      }
      
      return 1;
    }
   
  }

  

  

	  
  return 0;
}
