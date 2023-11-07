//#include "lib/token.cpp"
//#include "lib/parser.cpp"
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
            std::cout << tokens.at(i).token << " " << tokens[i].type << " " << tokens[i].line << " " << tokens[i].column << std::endl;
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
        std::ostringstream error;
        error << "Runtime error: condition is not a bool.";
        throw std::runtime_error(error.str());
        
    }

    return stod(raw);
}

std::string EvaluateExpression(std::vector<Token> tokens){
    //print for error checking:
    //std::cout << "B";
    
    if((int)tokens.size() == 0 || tokens.at(0).type == END){
        return "";
    }
   // std::cout << "EXEC " << tokens.size() << ": ";
    //PrintV(tokens);

    std::vector<Token> tempRow = tokens;
    if(tempRow.back().type != END){
        tempRow.push_back(Token{tempRow.back().line, tempRow.back().column+1,"END", END});
    }

    //std::cout <<std::endl<< "EVAL " << tempRow.size() << " :";
    //PrintV(tempRow);

    //std::cout << 
   // PrintV(tempRow);
    InfixParser parser = InfixParser(tempRow);
    //std::cout <<"THERE";
    //InfixParser infixParser = InfixParser(tokens));
    //parser.ParserFunc(tokens);
    //std::cout << "EXPR";
    return parser.calculate();
}


void EvaluateExpressionChunk(std::vector<Token> tokens){
    if(tokens.size() == 0){
        return;
    }
        //std::cout << "A";
        //PrintV(tokens);
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
    
    //std::cout << "CHUNK";
}

void ParseBlock(std::vector<Token>& tokens) {
    if(tokens.back().type != END){
        tokens.push_back(Token{tokens.back().line, tokens.back().column+1,"END", END});
    }
    //std::cout << "TOKENS:";
    //PrintV(tokens);
    bool prevCond = true;
    int i = 0;
    if((int)tokens.size() == 0) {
        return;
    }
    //std::cout << "NEXT: ";
    //PrintV(tokens);

    
    while (i < (int)tokens.size()) {
        //std::cout << "[" << i << "(" << tokens[i].token << ")" << "]"<< " " << std::endl;


        if (tokens[i].token == "print") {
            //std::cout << "PRINTBLOCK" << std::endl;
            i++;
            std::vector<Token> printExpr;
            while (i < (int)tokens.size()) {
                printExpr.push_back(tokens[i]);
                if(i == (int)tokens.size()-1 || tokens[i].line != tokens[i+1].line){
                    
                    break;
                }
                i++;
            }
            //std::cout << "printExpr: ";
            //PrintV(printExpr);
            std::cout << EvaluateExpression(printExpr)<< std::endl;
            //std::cout << "PRINT: " << i << tokens[i].token;

            //std::vector<Token> block(tokens.begin() + i, tokens.begin() + tokens.size());
            //PrintV(block);
            //int j = 0;
            //ParseBlock(block, j);
            i++;
        }



        else if (tokens[i].token == "while") {
            i++;
            //std::cout << "A";
            int conditionStart = i;
            while (tokens[i].token != "{") {
                //std::cout << "B";
                i++;
            }
            
            int conditionEnd = i;


            //std::cout << "C";
            int blockStart = i + 1;
            int blockParen = 1;
            while (blockParen > 0) {
               // std::cout << "D";
                i++;
                if (tokens[i].token == "{") {
                    blockParen++;
                }
                else if (tokens[i].token == "}") {
                    blockParen--;
                }
            }
            int blockEnd = i;
            //std::cout << "E";
            
            std::vector<Token> conditionExpr(tokens.begin() + conditionStart, tokens.begin() + conditionEnd);
            //std::cout << condToDouble(EvaluateExpression(conditionExpr)) << std::endl;
            while (condToDouble(EvaluateExpression(conditionExpr)) != 0) {
                //std::cout << "F";
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + blockEnd);
                //std::cout << "A" << std::endl;
                //PrintV(block);
                ParseBlock(block);
                //std::cout << "B" << std::endl;
            }
           // std::cout << "G";
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
                if (tokens[i].token == "{") blockParen++;
                else if (tokens[i].token == "}") blockParen--;
            }
            
            //std::cout << "BLOCKSTART:"<<blockStart << "BLOCKEND:"<<blockEnd<<std::endl;
            

            
            if (condToDouble(EvaluateExpression(conditionExpr)) != 0) {
                //std::cout << "TRUEIF" <<std::endl;
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i);
                //std::cout << "Block: ";
                //PrintV(block);
                
                ParseBlock(block);
                prevCond = true;
            }else{
                prevCond=false;
            } 
            i++;
            //std::cout << "IF: " << i << tokens[i].token;
        } 



        else if (tokens[i].token == "else if") {


            int conditionStart = i + 1;
            while (tokens[i].token != "{") {
                i++;
            }
            
            int conditionEnd = i;

            std::vector<Token> conditionExpr(tokens.begin() + conditionStart, tokens.begin() + conditionEnd);
        
    
            //std::cout << "A";
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


            //std::cout << "B";
            if(prevCond == false && condToDouble(EvaluateExpression(conditionExpr)) != 0){
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i);
                //std::cout << "C";
                
                ParseBlock(block);
                prevCond = true;
            } else {
                prevCond = false;
            }
            
            i++;
            
        }
        

        else if (tokens[i].token == "else") {
            i++;
            //std::cout << "A";
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
            //std::cout << "B";
            if(prevCond == false){
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i);
                //std::cout << "C";
                
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

        //EVALUATE EXPRESSIONS AND STORE VARS
        else {
           // std::cout << "H";
            //std::cout << "A";
            int blockStart = i;
            
            while(i < (int)tokens.size() && tokens[i].type != COMMAND && tokens[i].token != "}"){
                i++;
            }
            int blockEnd = i;
            
//std::cout << "B";
            std::vector<Token> block (tokens.begin() + blockStart, tokens.begin() + blockEnd);
            
            //std::cout << "C";
            //std::cout << "CHUNK: ";
            //PrintV(block);

            if((int)block.size() > 0 && block.at(0).type != END){
                //std::cout << "HERE";
                EvaluateExpressionChunk(block);
            }
            //i++; 
        }
    }

    return;
}

int main() {
    Lexer lexer = Lexer();


    std::vector<Token> tokens = lexer.lexer();
    /*if(tokens.at(0).token != "some_var"){
        PrintV(tokens);
    }*/
    try {
        ParseBlock(tokens);
    } catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    return 0;
}
