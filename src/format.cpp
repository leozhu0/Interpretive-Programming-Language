#include "lib/token.h"
#include "lib/parser.cpp"
#include "lib/lexer.cpp"
//#include "lib/infix.cpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <map>


void PrintV(std::vector<Token> tokens){
    if(tokens.size() > 0){
        for(int i = 0; i < (int)(tokens.size()); i++){
            std::cout << tokens.at(i).token <<" ";
        }
    }
    std::cout << std::endl;
}

double EvaluateExpression(std::vector<Token> tokens){
    //print for error checking:
    std::cout <<std::endl<< "EVAL " << tokens.size() << " :";
    PrintV(tokens);


    std::vector<Token> tempRow = tokens;
    if(tempRow.back().type != END){
        tempRow.push_back(Token{tempRow.back().line, tempRow.back().column+1,"END", END});
    }
    Parser parser = Parser();
    //InfixParser infixParser = InfixParser(tokens));
    parser.ParserFunc(tokens);
    return parser.calculate();
}


void EvaluateExpressionChunk(std::vector<Token> tokens){
    
    std::vector<std::vector<Token>> multilineTokens;
    
    int index = 0; 
    //If END is on a new line, then stop at the line before, otherwise do the whole thing
    for(int i = 0; i < tokens[tokens.size() - 1 - ((tokens[tokens.size()-1].column == 1)?1:0)].line; i++){
        std::vector<Token> tempRow;
        
        while(tokens[index].line == i+1){
            tempRow.push_back(tokens[index]);
            index++; 
        }
        multilineTokens.push_back(tempRow);
    }

    for (const auto &line : multilineTokens)
    {
        EvaluateExpression(line);
    }

    
}

void ParseBlock(std::vector<Token>& tokens, int& i) {
    
    if(tokens.size() == 0) {
        return;
    }
    std::cout << "NEXT: ";
    PrintV(tokens);

    std::cout << "[" << i << "(" << tokens[i].token << ")" << "]"<< " ";
    while (i < tokens.size()) {


        if (tokens[i].token == "print") {
            //std::cout << "PRINTBLOCK" << std::endl;
            i++;
            std::vector<Token> printExpr;
            while (i < tokens.size()) {
                printExpr.push_back(tokens[i]);
                if(tokens[i].line != tokens[i+1].line){
                    
                    break;
                }
                i++;
            }
            //std::cout << "printExpr: ";
            //PrintV(printExpr);
            std::cout << "Print: " << EvaluateExpression(printExpr)<< std::endl;
        std::cout << "PRINT: " << i << tokens[i].token;

            //std::vector<Token> block(tokens.begin() + i, tokens.begin() + tokens.size());
            //PrintV(block);
            //int j = 0;
            //ParseBlock(block, j);
        }



        else if (tokens[i].token == "while") {
            int start = i;
            while (tokens[i].token != "{") {
                i++;
            }
            int conditionStart = start + 1;
            int conditionEnd = i;

            
            std::vector<Token> conditionExpr(tokens.begin() + conditionStart, tokens.begin() + conditionEnd);
            while (EvaluateExpression(conditionExpr) > 0) {
                int blockStart = i + 1;
                int blockParen = 1;
                while (blockParen > 0) {
                    i++;
                    if (tokens[i].token == "{") {
                        blockParen++;
                    }
                    else if (tokens[i].token == "}") {
                        blockParen--;
                    }
                }
                int blockEnd = i;
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + blockEnd);
                int j = 0;
                ParseBlock(block, j);
            }
        } 
        

        
        
        else if (tokens[i].token == "if") {
            int conditionStart = i + 1;
            while (tokens[i].token != "{") {
                i++;
            }
            
            int conditionEnd = i;

            std::vector<Token> conditionExpr(tokens.begin() + conditionStart, tokens.begin() + conditionEnd);
            //std::cout << "COND: ";
            //PrintV(conditionExpr);




            int blockStart = i + 1;
            int blockParen = 1;
            while (blockParen > 0) {
                i++;
                if (tokens[i].token == "{") {
                    blockParen++;
                } else if (tokens[i].token == "}") {
                    blockParen--;
                }
            }
            
            //std::cout << "BLOCKSTART:"<<blockStart << "BLOCKEND:"<<blockEnd<<std::endl;
            

            
            if (EvaluateExpression(conditionExpr) > 0) {
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i);
                //std::cout << "Block: ";
                //PrintV(block);
                int j = 0;
                ParseBlock(block, j);
            } /*else { //IF the condition was false
                std::vector<Token> block(tokens.begin() + i, tokens.begin() + tokens.size());
                //std::cout << "Block2: ";
                //PrintV(block);
                int j = 0;
                ParseBlock(block, j);
            }*/
            i++;
            std::cout << "IF: " << i << tokens[i].token;
        } 
        
        
        else if (tokens[i].token == "else if") {
            
           int conditionStart = i + 1;
            while (tokens[i].token != "{") i++;
            
            int conditionEnd = i;

            
            std::vector<Token> conditionExpr(tokens.begin() + conditionStart, tokens.begin() + conditionEnd);
            
            
             int blockStart = i + 1;
            int blockEnd = i;
            int blockParen = 1;
            while (blockParen > 0) {
                blockEnd++;
                if (tokens[i].token == "{") blockParen++;
                else if (tokens[i].token == "}") blockParen--;
            }
            
            
            if (EvaluateExpression(conditionExpr) > 0) {
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + blockEnd);
                int j = 0;
                ParseBlock(block, j);
            } else {
                blockStart = blockEnd +1;
                blockEnd = tokens.size();
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + blockEnd);
                int j = 0;
                ParseBlock(block, j);
            }
        } 



        else if (tokens[i].token == "else") {
            
            int blockStart = i + 1;
            
            int blockParen = 1;

            while (blockParen > 0) {
                i++;
                if (tokens[i].token == "{") {
                    blockParen++;
                }
                else if (tokens[i].token == "}") {
                    blockParen--;
                }
            }

            int blockEnd = i;
            std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + blockEnd);
            int j = 0;
            ParseBlock(block, j);
        } 

        //EVALUATE EXPRESSIONS AND STORE VARS
        else {
            
            //std::cout << "A";
            int blockStart = i + 1;
            
            while(i < tokens.size() && tokens[i].type != COMMAND && tokens[i].token != "}"){
                i++;
            }
            int blockEnd = i;
            
//std::cout << "B";
            std::vector<Token> block (tokens.begin() + blockStart, tokens.begin() + blockEnd);
            
//std::cout << "C";
std::cout << "CHUNK: ";
PrintV(block);

            if(block.size() > 0 && block.at(0).type != END){
                EvaluateExpressionChunk(block);
            }
          
        }

        
       
    }

    return;
}

int main() {
    Lexer lexer = Lexer();

    std::vector<Token> tokens = lexer.lexer();


    for(int i = 0; i < (int)(tokens.size()); i++){
            std::cout << std::right << std::setw(4) << tokens.at(i).line << std::right << std::setw(5) << tokens.at(i).column << "  " << tokens.at(i).token <<std::endl;
    }

    int i = 0;
    ParseBlock(tokens, i);

    return 0;
}