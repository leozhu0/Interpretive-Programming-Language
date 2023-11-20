#include "lib/infix.cpp"//cpp
#include "lib/lexer.cpp"//cpp
#include "lib/value.cpp"
#include <iostream>

int main() {
  Lexer lexer = Lexer();
  std::string line;
  std::map<std::string, Value> variables;

  while (std::getline(std::cin, line)) {
    try {
      InfixParser infixParser = InfixParser(lexer.lexer(line), variables);
      std::cout << infixParser.toString() << std::endl << infixParser.calculate() << std::endl;
    }

    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }

  return 0;
}
