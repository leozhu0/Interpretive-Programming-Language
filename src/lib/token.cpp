#include <iostream>
#include "token.h"

TokenType Token::tokenType(char token){
    
    if(token == '.' || ((int)token >= 48 && (int)token <= 57) ){
        return NUMBER;
    }

    if(token == ')' || token == '(') {
        return PARENTHESIS;
    }

    if(token == '}' || token == '{') {
        return BLOCK;
    }

    if(token == '+' || token == '-' || token == '*' || token == '/' || token == '%') {
        return OPERATOR;
    }

    if(token == '&' || token == '^' || token == '|') {
        return LOGIC;
    }

    if(token == '>' || token == '<' || token == '!'){
        return COMPARE;
    }

    if(token== '='){
        return ASSIGNMENT;
    }

    if(token == '_' || ((int)token >= 65 && (int)token <= 90) || ((int)token >= 97 && (int)token <= 122) ){
        return VARIABLE;
    }

    if((int)token)

    if(isspace(token)){
        return SPACE;
    }

    return NULLTYPE;
}