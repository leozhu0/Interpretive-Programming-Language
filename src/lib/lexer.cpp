#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "token.h"
#include "lexer.h"


TokenType Lexer::tokentype(char token){
    
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

void Lexer::pushseq(std::string element, TokenType type, int line, int column, std::vector<Token> &sequence){
    if(element != ""){
        sequence.push_back(Token{line,column, element, type});
    }
}

    

Lexer::Lexer(std::string filename){
    file.open(filename);
}

std::vector<Token> Lexer::lexer(){
    std::vector<Token> sequence;
    std::string raw_input;
    int line = 1;
    while (std::getline(file, raw_input)) {
        int num_decimal = 0;
        std::string element = "";
        for(int i = 0; i < (int)raw_input.length(); i++){
            TokenType type = tokentype(raw_input[i]);

            if(type == NULLTYPE || (num_decimal > 0 && raw_input[i] == '.')){
                std::cout << "Syntax error on line "<< line <<" column "<< i+1 <<"." << std::endl;
                exit(1);
            }

            if(type != SPACE){
                if(raw_input[i] == '.' || ((int)raw_input[i] >= 48 && (int)raw_input[i] <= 57)) {
                    if(raw_input[i] == '.'){
                        num_decimal++;
                    }
                    element += raw_input[i];
                } else {
                    pushseq(element, NUMBER, line, i+1 - element.size(), sequence);
                    num_decimal = 0;
                    pushseq(raw_input.substr(i, 1), type, line, i+1, sequence);
                    element = "";
                }
            } else {
                pushseq(element, NUMBER, line, i+1 - element.size(), sequence);
                element = "";
            }

            if(i == (int)(raw_input.length()-1) && tokentype(element[0]) != NULLTYPE){ //ALWAYS NUMBER 
                sequence.push_back(Token{line,i+1,element, tokentype(element[0])});
            }

        }
        element = "";
        line++;
    }

    file.close();

    sequence.push_back(Token{line,1,"END", END});


    return sequence;
}







