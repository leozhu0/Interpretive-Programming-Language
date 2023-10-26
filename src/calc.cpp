#include "lib/infix.h"
#include "lib/lexer.h"
#include <iostream>

int main() {
  Lexer lexer;

  while (lexer = Lexer()) {
    try {
      InfixParser infixParser = InfixParser(lexer.lexer());
      std::cout << infixParser.toString() << std::endl << infixParser.calculate() << std::endl;
    }

    catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}
