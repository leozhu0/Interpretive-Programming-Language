#include <string>
#include <sstream>
#include <iostream>
#include "lexer.h"

//std::string thing = "";

void Lexer::pushSeq(std::string element, TokenType type, int line, int column, std::vector<Token> &sequence){
    if(element != ""){
        if(type==NUMBER){
            if(element[0]=='.'){
                std::cout << "Syntax error on line "<< line <<" column "<< int(column)<<"." <<std::endl;
                exit(1);
            }
            if(element.back() == '.'){
                std::cout << "Syntax error on line "<< line <<" column "<< (int)(column + element.size()) <<"."<<std::endl;
                exit(1);
            }
        }
        sequence.push_back(Token{line,column, element, type});
    }
}

std::vector<Token> Lexer::lexer(){
    std::vector<Token> sequence;
    char rawInput;
    int line = 1;
    int i = 1;
    int indents = 0;

    //This will later be used to check for more than one decimal in number
    int numDecimal = 0;
    std::string element = "";
    while (std::cin.get(rawInput)) {
        //thing +=  rawInput;
       //std::cout <<rawInput;
        if(rawInput == '\n'){
            indents++;

            //When you start a newline, take what was inside element and add it to sequence, this will always be a number or var
            if(Token::tokenType(element[0]) != NULLTYPE){ 
                pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
            }


            i=0;
            line++;
            element="";
        } else {
            indents = 0;
        }

        TokenType type = Token::tokenType(rawInput);

        //Decimal error (two decimals in the same num)
        if((numDecimal > 0 && rawInput == '.')){
            std::cout << "Syntax error on line "<< line <<" column "<< i<<"." << std::endl;
            exit(1);
        }

        if(type == NULLTYPE){
            std::cout << "Syntax error on line "<< line <<" column "<< i<<"." << std::endl;
            exit(1);
        }

        if(type != SPACE){
                if(Token::tokenType(rawInput)==NUMBER) {
                    if(Token::tokenType(element[0]) == NUMBER || element == ""){
                        if(rawInput == '.'){
                            numDecimal++;
                        }
                        element += rawInput;
                    } else if (Token::tokenType(element[0]) == VARIABLE){
                        if(rawInput == '.'){
                            std::cout << "Syntax error on line "<< line <<" column "<< i<<"." << std::endl;
                            exit(1);
                        }

                        element += rawInput;
                    } else {
                         //When the number input stops, insert it and then insert what you land on
                        pushSeq(element, NUMBER, line, i - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }
                    
                } else if(Token::tokenType(rawInput) == VARIABLE){
                    if(Token::tokenType(element[0]) == NUMBER){
                        std::cout << "Syntax error on line "<< line <<" column "<< i<<"." << std::endl;
                        exit(1);
                    } else if (Token::tokenType(element[0]) == VARIABLE || element == ""){
                        element += rawInput;
                    } else {
                        //When the variable input stops, insert it and then insert what you land on
                        pushSeq(element, VARIABLE, line, i - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }

                } else {
                    pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
                    numDecimal = 0;
                    std::string rawInputString(1, rawInput);
                    pushSeq(rawInputString, type, line, i, sequence);
                    element = "";
                }
                
        } else {
            pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
            element = "";
            numDecimal = 0;
        }

        i++;
    }

    //After termination, add whatever was stored in element to the sequence
    if(element != ""){
        pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
    }

    //Insert END (either new line or same line)
    if(sequence.size() == 0 || indents >= 1){
        sequence.push_back(Token{line,1,"END", END});
    } else {
        sequence.push_back(Token{line,sequence.back().column+1,"END", END});
    }


    /*if(thing[1] != '2'){
        std::cout << thing;
    }*/

    
    return sequence;
}


//Used for the Infix parser
std::vector<Token> Lexer::lexer(std::string raw){
    std::vector<Token> sequence;
    int line = 1;
    int i = 1;
    //This will later be used to check for more than one decimal in number
    int numDecimal = 0;
    std::string element = "";
    char rawInput;
    for(int r = 0; r <= (int)raw.length(); r++) {
        if(r == (int)raw.length()){
            if(Token::tokenType(element[0]) != NULLTYPE){ 
                pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
            }

            break;
        }
        
        rawInput = raw.at(r);
        TokenType type = Token::tokenType(rawInput);

        if(type == NULLTYPE || (numDecimal > 0 && rawInput == '.')){
            std::cout << "Syntax error on line "<< line <<" column "<< i <<"." << std::endl;
            exit(1);
        }

        if(type != SPACE){
                if(Token::tokenType(rawInput)==NUMBER) {
                    if(Token::tokenType(element[0]) == NUMBER || element == ""){
                        if(rawInput == '.'){
                            numDecimal++;
                        }
                        element += rawInput;
                    } else if (Token::tokenType(element[0]) == VARIABLE){
                        if(rawInput == '.'){
                            std::cout << "Syntax error on line "<< line <<" column "<< i <<"." << std::endl;
                            exit(1);
                        }
                        element += rawInput;
                    } else {
                        pushSeq(element, NUMBER, line, i - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }
                    
                } else if(Token::tokenType(rawInput)==VARIABLE){
                    if(Token::tokenType(element[0]) == NUMBER){
                        std::cout << "Syntax error on line "<< line <<" column "<< i <<"." << std::endl;
                        exit(1);
                    } else if (Token::tokenType(element[0]) == VARIABLE || element == ""){
                        element += rawInput;
                    } else {
                        pushSeq(element, VARIABLE, line, i - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }

                } else {
                    pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
                    numDecimal = 0;
                    std::string rawInputString(1, rawInput);
                    pushSeq(rawInputString, type, line, i, sequence);
                    element = "";
                }
                
                
    
        } else {
            pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
            element = "";
            numDecimal = 0;
        }

        i++;
    }

    sequence.push_back(Token{line,(int)raw.length()+1,"END", END});
    
    return sequence;
}