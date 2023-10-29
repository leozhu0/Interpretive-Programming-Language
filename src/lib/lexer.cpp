#include <string>
#include <sstream>
#include <iostream>
#include "lexer.h"

#include <ctime>

void Lexer::pushSeq(std::string element, TokenType type, int line, int column, std::vector<Token> &sequence){
    srand(static_cast<unsigned int>(time(nullptr)));
    if(element != ""){
        if(type==NUMBER){
        if(element[0]=='.'){
           
            std::cout << "Syntax error on line "<< line <<" column "<< int(column)<<"." <<std::endl;
            if(rand() % 2 == 0){
                std::cout <<":::"<<element<<std::endl;
            }
            exit(1);
        }
        if(element.back() == '.'){
            std::cout << "Syntax error on line "<< line <<" column "<< (int)(column + element.size()-1) <<"." << std::endl;
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
    //TokenType elementType = NULLTYPE;
    //bool lastWasSpace = 0;
    //int numLineChars = 0;
    while (std::cin.get(rawInput)) {
        //std::cout <<rawInput;
        //numLineChars ++;
        if(rawInput == '\n'){
            //numLineChars = 0;
            indents++;
           /* if(indents > 1){
                //std::cout << "EARLY"<<std::endl;
                sequence.push_back(Token{line,1,"END", END});
                return sequence;
                //break;
            }*/

            //When you start a newline, take what was inside element and add it to sequence. 
            //This will always be a number
            if(Token::tokenType(element[0]) != NULLTYPE){ 
                //std::cout << "A: " << i + 1 - element.size() << element << ":::";
                pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
            }


            i=0;
            line++;
            element="";
        } else {
            indents = 0;
        }

        TokenType type = Token::tokenType(rawInput);

        if(type == NULLTYPE || (numDecimal > 0 && rawInput == '.')){
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
                            exit(0);
                        }
                        element += rawInput;
                    } else {
                        //std::cout << "B: " << i - element.size();
                        pushSeq(element, NUMBER, line, i - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        //std::cout << "C: " << i;
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }
                    
                } else if(Token::tokenType(rawInput)==VARIABLE){
                    if(Token::tokenType(element[0]) == NUMBER){
                        std::cout << "Syntax error on line "<< line <<" column "<< i<<"." << std::endl;
                        exit(0);
                    } else if (Token::tokenType(element[0]) == VARIABLE || element == ""){
                        element += rawInput;
                    } else {
                        pushSeq(element, VARIABLE, line, i - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        //std::cout << "D: " << i;
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }

                } else {
                    //std::cout << "E: " <<i - element.size();
                    pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
                    numDecimal = 0;
                    std::string rawInputString(1, rawInput);
                    //std::cout << "F: " <<i;
                    pushSeq(rawInputString, type, line, i, sequence);
                    element = "";
                    //elementType = NULLTYPE;
                }
                
                
    
        } else {
            //std::cout << "G: " << i - element.size();
            pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
            element = "";
            numDecimal = 0;
        }

        /*if(type != SPACE || !lastWasSpace){
            i++;
        }
        if(type == SPACE){
            lastWasSpace=1;
        } else{
            lastWasSpace = 0;
        }*/
        i++;
    }

    /*if(sequence.size() > 0){
        sequence.push_back(Token{line,sequence.back().column + 1,"END", END});
    } else {
        sequence.push_back(Token{line,1,"END", END});
    }*/
    if(sequence.size() == 0 || indents >= 1){
        sequence.push_back(Token{line,1,"END", END});
    } else {
        sequence.push_back(Token{line,sequence.back().column+1,"END", END});
        //
    }
    


    return sequence;
}



std::vector<Token> Lexer::lexer(std::string raw){
    
    std::vector<Token> sequence;
    int line = 1;
    int i = 1;
    //This will later be used to check for more than one decimal in number
    int numDecimal = 0;
    std::string element = "";
    //int numlineChars = 0;
    //TokenType elementType = NULLTYPE;
    //bool lastWasSpace = 0;
    char rawInput;
    for(int r = 0; r <= (int)raw.length(); r++) {

         

        if(r == (int)raw.length()){
            if(Token::tokenType(element[0]) != NULLTYPE){ 
                //std::cout << "A: " << i + 1 - element.size() << element << ":::";
                pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);

            }

       
            break;
        }
        
        
        rawInput = raw.at(r);
        //std::cout << rawInput << std::endl;
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
                            exit(0);
                        }
                        element += rawInput;
                    } else {
                        //std::cout << "B: " << i - element.size();
                        pushSeq(element, NUMBER, line, i - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        //std::cout << "C: " << i;
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }
                    
                } else if(Token::tokenType(rawInput)==VARIABLE){
                    if(Token::tokenType(element[0]) == NUMBER){
                        std::cout << "Syntax error on line "<< line <<" column "<< i <<"." << std::endl;
                        exit(0);
                    } else if (Token::tokenType(element[0]) == VARIABLE || element == ""){
                        element += rawInput;
                    } else {
                        pushSeq(element, VARIABLE, line, i - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        //std::cout << "D: " << i;
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }

                } else {
                    //std::cout << "E: " <<i - element.size();
                    pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
                    numDecimal = 0;
                    std::string rawInputString(1, rawInput);
                    //std::cout << "F: " <<i;
                    pushSeq(rawInputString, type, line, i, sequence);
                    element = "";
                    //elementType = NULLTYPE;
                }
                
                
    
        } else {
            //std::cout << "G: " << i - element.size();
            pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
            element = "";
            numDecimal = 0;
        }

        /*if(type != SPACE || !lastWasSpace){
            i++;
        }
        if(type == SPACE){
            lastWasSpace=1;
        } else{
            lastWasSpace = 0;
        }*/
        i++;
    }

    /*if(sequence.size() > 0){
        sequence.push_back(Token{line,sequence.back().column + 1,"END", END});
    } else {
        sequence.push_back(Token{line,1,"END", END});
    }*/
    
    sequence.push_back(Token{line,(int)raw.length()+1,"END", END});
    
    


    return sequence;
}





 
/*
x 7.899-f(temp)    
_67*)_+B =
  lot49/50
(())
_v4rl4b13=42
*/