#pragma once
#include <string>

enum TokenType {
  NUMBER,
  OPERATOR,
  PARENTHESIS,
  END,
  NULLTYPE,
  SPACE,
  VARIABLE,
  ASSIGNMENT,
  LOGIC,
  COMPARE,
  COMMAND,
  BLOCK,
  BOOL,
  COMMA,
  SEMICOLON
};

class Token {
  public:  
    static TokenType tokenType(char token);
    int line;
    int column;
    std::string token;
    TokenType type;
  
};