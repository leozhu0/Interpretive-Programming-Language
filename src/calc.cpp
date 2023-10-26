#include "lib/infix.h"
#include "lib/lexer.h"
#include <iostream>

int main() {
  Lexer lexer = Lexer();

  InfixParser infixParser = InfixParser(lexer.lexer());

  std::cout << infixParser.toString() << std::endl << infixParser.calculate() << std::endl;
	  
  return 0;
}
