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
  ASSIGNMENT
};

class Token {
  public:   
    int line;
    int column;
    std::string token;
    TokenType type;

    /*Token(int a, int b, std::string c, TokenType d){
      line = a;
      column = b;
      token = c;
      type = d;
    }*/
  
};