#include "lib/token.h"
#include "lib/parser.h"
#include "lib/lexer.h"
#include <iostream>

int main() {
  Lexer lexer = Lexer();

  Parser parser = Parser(lexer.lexer());

  std::cout << parser.toString() << std::endl << parser.calculate();
	  
  return 0;
}
