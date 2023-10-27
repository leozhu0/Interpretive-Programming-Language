#include "lib/infix.cpp"
#include "lib/lexer.cpp"
#include <iostream>

int main() {
  Lexer lexer = Lexer();
  std::string line;

  while (std::getline(std::cin, line)) {
    try {
      InfixParser infixParser = InfixParser(lexer.lexer(line));
      std::cout << infixParser.toString() << std::endl << infixParser.calculate() << std::endl;
    }

    catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}
