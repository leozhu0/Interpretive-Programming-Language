#include <string>
#include <sstream>
#include <iostream>
#include "lexer.h"

TokenType Lexer::tokenType(char token){
    
    if(token == '.' || ((int)token >= 48 && (int)token <= 57) ){
        return NUMBER;
    }

    if(token == ')' || token == '(') {
        return PARENTHESIS;
    }

    if(token == '+' || token == '-' || token == '*' || token == '/') {
        return OPERATOR;
    }

    if(isspace(token)){
        return SPACE;
    }

    return NULLTYPE;
}

void Lexer::pushSeq(std::string element, TokenType type, int line, int column, std::vector<Token> &sequence){
    if(type==NUMBER){
        if(element[0]=='.'){
            std::cout << "Syntax error on line "<< line <<" column "<< column <<"." << std::endl;
            exit(1);
        }
        if(element.back() == '.'){
            std::cout << "Syntax error on line "<< line <<" column "<< column + element.size() <<"." << std::endl;
            exit(1);
        }
    }
    if(element != ""){
        sequence.push_back(Token{line,column, element, type});
    }
}

std::vector<Token> Lexer::lexer(){
    std::vector<Token> sequence;
    std::string rawInput;
    int line = 1;
    while (std::getline(std::cin, rawInput)) {
        //This will later be used to check for more than one decimal in number
        int numDecimal = 0;
        std::string element = "";
        for(int i = 0; i < (int)rawInput.length(); i++){
            TokenType type = tokenType(rawInput[i]);

            if(type == NULLTYPE || (numDecimal > 0 && rawInput[i] == '.')){
                std::cout << "Syntax error on line "<< line <<" column "<< i+1 <<"." << std::endl;
                exit(1);
            }

            if(type != SPACE){
                if(rawInput[i] == '.' || ((int)rawInput[i] >= 48 && (int)rawInput[i] <= 57)) {
                    if(rawInput[i] == '.'){
                        numDecimal++;
                    }
                    element += rawInput[i];
                } else {
                    pushSeq(element, NUMBER, line, i+1 - element.size(), sequence);
                    numDecimal = 0;
                    pushSeq(rawInput.substr(i, 1), type, line, i+1, sequence);
                    element = "";
                }

            } else {
                pushSeq(element, NUMBER, line, i+1 - element.size(), sequence);
                element = "";
                numDecimal = 0;
            }

            //This will always be a number
            if(i == (int)(rawInput.length()-1) && tokenType(element[0]) != NULLTYPE){ 
                pushSeq(element, tokenType(element[0]), line, i+2 - element.size(), sequence);
            }

        }
        element = "";
        line++;
    }

    if(sequence.size() > 0 && sequence.back().type == PARENTHESIS){
        sequence.push_back(Token{line-1,sequence.back().column + 1,"END", END});
    } else {
        sequence.push_back(Token{line,1,"END", END});
    }
    


    return sequence;
}







