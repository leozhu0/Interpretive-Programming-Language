#include <string>
#include <iostream>
#include <vector>
#include "parser.h"
#include "token.h"

bool validToken(char token){
    if(token == '.' || ((int)token >= 48 && (int)token <= 57) || token == ')' || token == '('){
        return true;
    }

    return false;
}

std::vector<Token> lexer(std::string raw_input){
    std::vector<Token> sequence;
    std::string element = "";
    for(int i = 0; i < raw_input.length(); i++){

        if(validToken(raw_input[i])){
            if(raw_input[i] == '.' || ((int)raw_input[i] >= 48 && (int)raw_input[i] <= 57)) {
                element += raw_input[i];
            } else {
                sequence.push_back(Token{0,0,element, NUMBER});
                sequence.push_back(Token{0,0,raw_input.substr(i, 1), OPERATOR});
                element = "";
            }
        }
        if(i == raw_input.length()-1){ 
            sequence.push_back(Token{0,0,element, NUMBER});
        }
        

    }
    return sequence;
}

int main() {
    std::vector<Token> seq= lexer("(7.0+8.9)+5");
    for(int i = 0; i < seq.size(); i++){
        std::cout << "> " << seq.at(i).token << std::endl;
    }
    return 0;
}