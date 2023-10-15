#include "lib/token.h"
#include "lib/parser.cpp"
#include "lib/lexer.cpp"
#include <iostream>

int main() {
  Lexer lexer = Lexer("code.txt");

  Parser parser;
  parser.createTree(lexer.lexer());

  std::cout << parser.toString() << std::endl << parser.calculate();
	  
  return 0;
}
