#include "lib/infix.h"//cpp
#include "lib/lexer.h"//cpp
#include "lib/value.h"
#include <iostream>

int main() {
  Lexer lexer = Lexer();
  std::string line;
  std::map<std::string, Value> variables;

  variables["len"] = nullptr;
  variables["pop"] = nullptr;
  variables["push"] = nullptr;

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
