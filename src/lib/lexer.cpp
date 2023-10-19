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
    char rawInput;
    int line = 1;
    int i = 0;
    int indents = 0;
    //This will later be used to check for more than one decimal in number
    int numDecimal = 0;
    std::string element = "";
    TokenType elementType = NULLTYPE;
    while (std::cin.get(rawInput)) {
        if(rawInput == '\n'){
            indents++;
            if(indents > 1){
                sequence.push_back(Token{line,1,"END", END});
                return sequence;
                //break;
            }

            //When you start a newline, take what was inside element and add it to sequence. 
            //This will always be a number
            if(tokenType(element[0]) != NULLTYPE){ 
                pushSeq(element, tokenType(element[0]), line, i+2 - element.size(), sequence);
            }


            i=0;
            line++;
            element="";
        } else {
            indents = 0;
        }

        TokenType type = tokenType(rawInput);

        if(type == NULLTYPE || (numDecimal > 0 && rawInput == '.')){
            std::cout << "Syntax error on line "<< line <<" column "<< i+1 <<"." << std::endl;
            exit(1);
        }

        if(type != SPACE){
                if(tokenType(rawInput)==NUMBER) {
                    if(tokenType(element[0]) == NUMBER || element == ""){
                        if(rawInput == '.'){
                            numDecimal++;
                        }
                        element += rawInput;
                    } else if (tokenType(element[0]) == VARIABLE){
                        if(rawInput == '.'){
                            std::cout << "Syntax error on line "<< line <<" column "<< i+1 <<"." << std::endl;
                            exit(0);
                        }
                        element += rawInput;
                    } else {
                        pushSeq(element, NUMBER, line, i+1 - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        pushSeq(rawInputString, type, line, i+1, sequence);
                        element = "";
                    }
                    
                } else if(tokenType(rawInput)==VARIABLE){
                    if(tokenType(element[0]) == NUMBER){
                        std::cout << "Syntax error on line "<< line <<" column "<< i+1 <<"." << std::endl;
                        exit(0);
                    } else if (tokenType(element[0]) == VARIABLE || element == ""){
                        element += rawInput;
                    } else {
                        pushSeq(element, VARIABLE, line, i+1 - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        pushSeq(rawInputString, type, line, i+1, sequence);
                        element = "";
                    }

                } else {
                    pushSeq(element, tokenType(element[0]), line, i+1 - element.size(), sequence);
                    numDecimal = 0;
                    std::string rawInputString(1, rawInput);
                    pushSeq(rawInputString, type, line, i+1, sequence);
                    element = "";
                    //elementType = NULLTYPE;
                }
                
                

        } else {
            pushSeq(element, tokenType(element[0]), line, i+1 - element.size(), sequence);
            element = "";
            numDecimal = 0;
        }

        

        i++;
    }

    if(sequence.size() > 0){
        sequence.push_back(Token{line-1,sequence.back().column + 1,"END", END});
    } else {
        sequence.push_back(Token{line,1,"END", END});
    }
    


    return sequence;
}







