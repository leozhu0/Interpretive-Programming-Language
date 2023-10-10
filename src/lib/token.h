#include <string>

enum TokenType {
  NUMBER,
  OPERATOR,
  PARENTHESIS,
  END  
};

class Token {
  int line;
  int column;
  std::string token;
  TokenType type;

  // add functions if need be for lexer
  
};
