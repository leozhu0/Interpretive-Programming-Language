#include "lib/lexer.h" // cpp
#include "lib/infix.h" //cpp
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>



void PrintV(std::vector<Token> tokens){
    std::cout << "_______" << std::endl;
    if((int)tokens.size() > 0){
        for(int i = 0; i < (int)(tokens.size()); i++){
            std::cout << tokens.at(i).token << " ";
        }
    }
    std::cout << "_______" << std::endl;
}


double condToDouble(std::string raw){
    if(raw == "true"){
        return 1;
    }

    else if(raw=="false"){
        return 0;
    }

    else {
       std::cout << "Runtime error: condition is not a bool." << std::endl;
       exit(3);
    }

    return stod(raw);
}

std::string EvaluateExpression(std::vector<Token> tokens){
    //Return nothing if tokens vector is empty or just END
    if((int)tokens.size() == 0 || tokens.at(0).type == END){
        return "";
    }

    std::vector<Token> tempRow = tokens;
    if(tempRow.back().type != END){
        tempRow.push_back(Token{tempRow.back().line, tempRow.back().column+1,"END", END});
    }

    InfixParser parser = InfixParser(tempRow);
    return parser.calculate();
}


void EvaluateExpressionChunk(std::vector<Token> tokens){
    if(tokens.size() == 0){
        return;
    }
    std::vector<std::vector<Token>> multilineTokens;
    
    int index = 0; 

    //If END is on a new line, then stop at the line before, otherwise do the whole thing
    for(int i = 0; i < tokens[(int)tokens.size() - 1 - ((tokens[(int)tokens.size()-1].column == 1)?1:0)].line; i++){
        std::vector<Token> tempRow;
        
        while((int)tokens.size() > index && tokens[index].line == i+1){
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

void ParseBlock(std::vector<Token>& tokens) {
    //The last token must always have an end (to know when to terminate later)
    if(tokens.back().type != END){
        tokens.push_back(Token{tokens.back().line, tokens.back().column+1,"END", END});
    }

    //prevCond is used for else and else if
    bool prevCond = true;
    int i = 0;
    if((int)tokens.size() == 0) {
        return;
    }
    
    //Iterativly go through each token (i changes within loop based on the code)
    while (i < (int)tokens.size()) {
        if (tokens[i].token == "print") {
            i++;
            std::vector<Token> printExpr;
            while (i < (int)tokens.size()) {
                printExpr.push_back(tokens[i]);
                if(i == (int)tokens.size()-1 || tokens[i].line != tokens[i+1].line){
                    break;
                }
                i++;
            }

            std::cout << EvaluateExpression(printExpr)<< std::endl;
            i++;
        }

        else if (tokens[i].token == "while") {
            i++;
            int conditionStart = i;
            while (tokens[i].token != "{") {
                i++;
            }
            
            int conditionEnd = i;
            int blockStart = i + 1;
            int blockParen = 1;
            while (blockParen > 0) {
                i++;
                if (tokens[i].token == "{") blockParen++;
                else if (tokens[i].token == "}") blockParen--;
            }

            int blockEnd = i;
            std::vector<Token> conditionExpr(tokens.begin() + conditionStart, tokens.begin() + conditionEnd);
            while (condToDouble(EvaluateExpression(conditionExpr)) != 0) {
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + blockEnd);
                ParseBlock(block);
            }
           
        } 
        
        else if (tokens[i].token == "if") {
            int conditionStart = i + 1;
            while (tokens[i].token != "{") {
                i++;
            }
            
            std::vector<Token> conditionExpr(tokens.begin() + conditionStart, tokens.begin() + i);
            int blockStart = i + 1;
            int blockParen = 1;
            while (blockParen > 0) {
                i++;
                if (tokens[i].token == "{") blockParen++;
                else if (tokens[i].token == "}") blockParen--;
            }
            
            if (condToDouble(EvaluateExpression(conditionExpr)) != 0) {
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i); 
                ParseBlock(block);
                prevCond = true;
            } else{
                prevCond = false;
            } 
            i++;
        } 

        else if (tokens[i].token == "else if") {
            int conditionStart = i + 1;
            while (tokens[i].token != "{") {
                i++;
            }
            
            int conditionEnd = i;
            std::vector<Token> conditionExpr(tokens.begin() + conditionStart, tokens.begin() + conditionEnd);
            int blockStart = i + 1;
            int blockParen = 1;
            while (blockParen > 0) {
                i++;
                if (tokens[i].token == "{") blockParen++;
                else if (tokens[i].token == "}") blockParen--;
            }

            if(prevCond == false && condToDouble(EvaluateExpression(conditionExpr)) != 0){
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i);
                ParseBlock(block);
                prevCond = true;
            }
            i++;
        }
        
        else if (tokens[i].token == "else") {
            i++; 
            int blockStart = i + 1;
            int blockParen = 1;

            while (blockParen > 0) {
                i++;
                if (tokens[i].token == "{") blockParen++;
                else if (tokens[i].token == "}") blockParen--;
            }
            
            if(prevCond == false){
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i);
                ParseBlock(block);
            }
            prevCond = true;
        } 
        
        else if (tokens[i].token == "}"){
            i++;
        } 
        
        else if(tokens[i].type == END){
            return;
        }

        //Evalutate expressions that are not commands (And create vars)
        else {
            int blockStart = i;
            
            while(i < (int)tokens.size() && tokens[i].type != COMMAND && tokens[i].token != "}"){
                i++;
            }
            int blockEnd = i;
            
            std::vector<Token> block (tokens.begin() + blockStart, tokens.begin() + blockEnd);
            
            if((int)block.size() > 0 && block.at(0).type != END){
                EvaluateExpressionChunk(block);
            }
            
        }
    }

    return;
}

int main() {
    std::vector<Token> tokens;
    try {
        Lexer lexer = Lexer();
        tokens = lexer.lexer();
    }
    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      exit(1);
    }

    try {
        ParseBlock(tokens);
    } 
    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      exit(2);
    }

    return 0;
}
