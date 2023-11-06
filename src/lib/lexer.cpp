#include <string>
#include <sstream>
#include <iostream>
#include "lexer.h"
#include <sstream>
#include <stdexcept>


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

        if(element == "else "){
            element = "else";
        }

        if(element == "while" || element == "if" || element == "print" || element == "else" || element == "else if"){
            sequence.push_back(Token{line,column, element, COMMAND});
        } else if(element == "true" || element == "false"){
            sequence.push_back(Token{line,column, element, BOOL});
        } else if(element == "<=" || element == ">=" || element == "==" || element == ">" || element == "<" || element == "!="){
            sequence.push_back(Token{line,column, element, COMPARE});
        } else {
            sequence.push_back(Token{line,column, element, type});
        }

        

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
        std::cout <<rawInput;
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
        if((numDecimal > 0 && rawInput == '.') || type == NULLTYPE){
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
                        if(element == "else " && rawInput == 'i'){
                            element = "else i";
                        }

                        else if (element == "else f" && rawInput == 'f'){
                            pushSeq("else if", VARIABLE, line, i - element.size(), sequence);
                            numDecimal = 0;
                            element = "";
                        }

                        else {
                            pushSeq(element, VARIABLE, line, i - element.size(), sequence);
                            numDecimal = 0;
                            std::string rawInputString(1, rawInput);
                            pushSeq(rawInputString, type, line, i, sequence);
                            element = "";
                        } 
                        


                    }

                } else {
                    //////////////////////////////////EDITS ALL HERE/////////////
                    if(Token::tokenType(element[0])!=LOGIC && Token::tokenType(element[0])!=COMPARE && element[0] != '=' && rawInput != '='){
                        //std::cout << "A";
                        pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
                        numDecimal = 0;
                        element="";
                    }

                    if(element == "=" && rawInput != '='){
                        //std::cout << "B";
                        pushSeq("=", ASSIGNMENT, line, i, sequence);
                        element = rawInput;
                    }

                    else if((element == "<" || element == "=" || element == ">" || element == "!") && rawInput == '='){
                        //std::cout << "C";
                        pushSeq(element+rawInput, COMPARE, line, i-1, sequence);
                        element = "";
                    }

                    else if((element == "<" || element == ">" || element == "!") && rawInput != '='){
                        //std::cout << "D";
                        pushSeq(element, COMPARE, line, i-1, sequence);
                        element = rawInput;
                    } 

                    else if(Token::tokenType(rawInput)==COMPARE || rawInput == '='){
                        //std::cout << "F";
                        pushSeq(element, Token::tokenType(element[0]), line, i-element.size(), sequence);
                        element = rawInput;
                    }
                    
                    else {
                        //std::cout << "E";
                        std::string rawInputString(1, rawInput);
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }
                    //////////////////////EDITS END
                }
                
        } else {
             if(element == "else if" || element.substr(0, 4) != "else"){
                //std::cout << "H";
                pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
                element = "";
                numDecimal = 0;
             } else {
                element += " ";
             }
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

    return sequence;
}



void Lexer::pushSeqThrow(std::string element, TokenType type, int line, int column, std::vector<Token> &sequence){
    if(element != ""){
        if(type==NUMBER){
            if(element[0]=='.'){
                std::ostringstream error;
                error << "Syntax error on line "<< line <<" column "<< int(column)<<".";
                throw std::runtime_error(error.str());
            }
            if(element.back() == '.'){
                std::ostringstream error;
                error << "Syntax error on line "<< line <<" column "<< (int)(column + element.size()) <<".";
                throw std::runtime_error(error.str());
            }
        }


        if(element == "else "){
            element = "else";
        }


        if(element == "while" || element == "if" || element == "print" || element == "else" || element == "else if"){
            sequence.push_back(Token{line,column, element, COMMAND});
        } else if(element == "true" || element == "false"){
            sequence.push_back(Token{line,column, element, BOOL});
        }else if(element == "<=" || element == ">=" || element == "==" || element == ">" || element == "<" || element == "!="){
            sequence.push_back(Token{line,column, element, COMPARE});
        }
        else {
            sequence.push_back(Token{line,column, element, type});
        }
    }
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
                pushSeqThrow(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
            }

            break;
        }
        
        rawInput = raw.at(r);
        TokenType type = Token::tokenType(rawInput);

       

        if(type == NULLTYPE || (numDecimal > 0 && rawInput == '.')){
            std::ostringstream error;
            error << "Syntax error on line "<< line <<" column "<< i <<".";
            throw std::runtime_error(error.str());
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
                            std::ostringstream error;
                            error << "Syntax error on line "<< line <<" column "<< i <<".";
                            throw std::runtime_error(error.str());
                        }
                        element += rawInput;
                    } else {
                        pushSeqThrow(element, NUMBER, line, i - element.size(), sequence);
                        numDecimal = 0;
                        std::string rawInputString(1, rawInput);
                        pushSeqThrow(rawInputString, type, line, i, sequence);
                        element = "";
                    }
                    
                } else if(Token::tokenType(rawInput)==VARIABLE){
                    if(Token::tokenType(element[0]) == NUMBER){
                        std::ostringstream error;
                        error << "Syntax error on line "<< line <<" column "<< i <<".";
                        throw std::runtime_error(error.str());
                    } else if (Token::tokenType(element[0]) == VARIABLE || element == ""){
                        element += rawInput;
                    } else {
                          //When the variable input stops, insert it and then insert what you land on
                        if(element == "else " && rawInput == 'i'){
                            element = "else i";
                        }

                        else if (element == "else f" && rawInput == 'f'){
                            pushSeq("else if", VARIABLE, line, i - element.size(), sequence);
                            numDecimal = 0;
                            element = "";
                        }

                        else {
                            pushSeq(element, VARIABLE, line, i - element.size(), sequence);
                            numDecimal = 0;
                            std::string rawInputString(1, rawInput);
                            pushSeq(rawInputString, type, line, i, sequence);
                            element = "";
                        } 
                        
                    }

                } else {
                    //////////////////////////////////EDITS ALL HERE/////////////
                    if(Token::tokenType(element[0])!=LOGIC && Token::tokenType(element[0])!=COMPARE && element[0] != '=' && rawInput != '='){
                        //std::cout << "A";
                        pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
                        numDecimal = 0;
                        element="";
                    }

                    if(element == "=" && rawInput != '='){
                        //std::cout << "B";
                        pushSeq("=", ASSIGNMENT, line, i, sequence);
                        element = rawInput;
                    }

                    else if((element == "<" || element == "=" || element == ">" || element == "!") && rawInput == '='){
                        //std::cout << "C";
                        pushSeq(element+rawInput, COMPARE, line, i-1, sequence);
                        element = "";
                    }

                    else if((element == "<" || element == ">" || element == "!") && rawInput != '='){
                        //std::cout << "D";
                        pushSeq(element, COMPARE, line, i-1, sequence);
                        element = rawInput;
                    } 

                    else if(Token::tokenType(rawInput)==COMPARE || rawInput == '='){
                        //std::cout << "F";
                        pushSeq(element, Token::tokenType(element[0]), line, i-element.size(), sequence);
                        element = rawInput;
                    }
                    
                    else {
                        //std::cout << "E";
                        std::string rawInputString(1, rawInput);
                        pushSeq(rawInputString, type, line, i, sequence);
                        element = "";
                    }
                    //////////////////////EDITS END
                }
                
                
    
        } else {
            if(element == "else if" || element.substr(0, 4) != "else"){
                //std::cout << "H";
                pushSeq(element, Token::tokenType(element[0]), line, i - element.size(), sequence);
                element = "";
                numDecimal = 0;
             } else {
                element += " ";
             }
            numDecimal = 0;
        }

        i++;
    }

    sequence.push_back(Token{line,(int)raw.length()+1,"END", END});
    
    return sequence;
}