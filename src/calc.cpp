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
      std::vector<Token> tempTokens = lexer.lexer(line);

      std::cout << "___start___" << std::endl;
      for (Token token : tempTokens)  {
        std::cout << "token: " << token.token << std::endl;
	std::cout << "type " << token.type << std::endl;
	std::cout << "line: " << token.line << std::endl;
	std::cout << "column: " << token.column << std::endl << std::endl;
      }
      std::cout << "___end___" << std::endl;

      InfixParser infixParser = InfixParser(lexer.lexer(line), variables);
      std::cout << infixParser.toString() << std::endl << infixParser.calculate() << std::endl;
    }

    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }

  return 0;
}
