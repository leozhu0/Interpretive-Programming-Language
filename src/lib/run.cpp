#include "run.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <variant>

void Scrypt::printV(std::vector<Token> tokens){
    std::cout << "_______" << std::endl;
    if((int)tokens.size() > 0){
        for(int i = 0; i < (int)(tokens.size()); i++){
            std::cout << tokens.at(i).token << " ";
        }
    }
    std::cout << "_______" << std::endl;
}

bool Scrypt::isBool(Value value){
	if(std::holds_alternative<bool>(value)){
		if(value == Value(true)) return true;
		else return false;
	}

	std::ostringstream error;
        error << "Runtime error: condition is not a bool.";
        throw std::runtime_error(error.str());
	
	//This will never happen
	return true;

}

Value Scrypt::evaluateExpression(std::vector<Token> tokens, std::map<std::string, Value>& variables){
    //Return nothing if tokens vector is empty or just END
    if((int)tokens.size() == 0 || tokens.at(0).type == END){
        return Value();
    }

    std::vector<Token> tempRow = tokens;
    if(tempRow.back().type != END){
        tempRow.push_back(Token{tempRow.back().line, tempRow.back().column+1,"END", END});
    }

    InfixParser parser = InfixParser(tempRow, variables);
    return parser.calculate();
}


void Scrypt::evaluateExpressionChunk(std::vector<Token> tokens, std::map<std::string, Value>& variables){
    if(tokens.size() == 0){
        return;
    }
    std::vector<std::vector<Token>> multilineTokens;
    


    for(int i = 0; i < (int)tokens.size(); i++){
        std::vector<Token> tempRow;
        while(i < (int)tokens.size() && tokens[i].token != ";"){
            tempRow.push_back(tokens[i]);
            i++;
        }
        multilineTokens.push_back(tempRow);
    }



    for (const auto &line : multilineTokens)
    {
        evaluateExpression(line, variables);
    }
}

bool Scrypt::isKeyword(Token token){
    if(token.token == "print" ||
    token.token == "def" ||
    token.token == "return" ||
    token.token == "while" ||
    token.token == "else" ||
    token.token == "else if" ||
    token.token == "if"){
        return true;
    }
    return false;
}

Value Scrypt::parseBlock(std::vector<Token>& tokens, std::map<std::string, Value>& variables, bool inFunc) {
    /*variables["len"] = std::make_shared<Value>(Value(nullptr));
    variables["pop"] = std::make_shared<Value>(Value(nullptr));
    variables["push"] = std::make_shared<Value>(Value(nullptr));
*/
    //The last token must always have an end (to know when to terminate later)
    if(tokens.back().type != END){
        tokens.push_back(Token{tokens.back().line, tokens.back().column+1,"END", END});
    }

    //prevCond is used for else and else if
    bool prevCond = true;
    int i = 0;
    if((int)tokens.size() == 0) {
        return nullptr;//nullptr;
    }
    
    //Iterativly go through each token (i changes within loop based on the code)
    while (i < (int)tokens.size()) {
        if (tokens[i].token == "print") {
            i++;
            std::vector<Token> printExpr;
            while (i < (int)tokens.size()) {
		if(tokens[i].token == ";"){break;}
                //printExpr.push_back(tokens[i]);
                if(isKeyword(tokens[i])){
                    std::cout <<"ERROR, keyword before ;"<<std::endl;
                    break;
                }
 		printExpr.push_back(tokens[i]);
                //if(tokens[i].token == ";"){ break; }
                i++;
            }

	    
	    if((int)printExpr.size() > 0){
            	std::cout << evaluateExpression(printExpr, variables) << std::endl;
	    }
            i++;
        }

        if (tokens[i].token == "return") {
	    if(!inFunc){
		    std::ostringstream error;
		    error << "Runtime error: unexpected return.";
		    throw std::runtime_error(error.str());
	    }

            i++;
            std::vector<Token> returnExpr;
            while (i < (int)tokens.size()) {
                //returnExpr.push_back(tokens[i]);
		if(tokens[i].token == ";"){break;}
                if(isKeyword(tokens[i])){
                    std::cout <<"ERROR, keyword before ;"<<std::endl;
                    break;
                }
                returnExpr.push_back(tokens[i]);
                //if(tokens[i].token == ";"){ break; }
                i++;
            }

	    if((int)returnExpr.size() == 0){
	    	return nullptr;
	    }
            return evaluateExpression(returnExpr, variables);
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
            while (isBool(evaluateExpression(conditionExpr, variables)) == true) {
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + blockEnd);
                parseBlock(block, variables, inFunc);
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
            
            if (isBool(evaluateExpression(conditionExpr, variables)) == true) {
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i); 
                parseBlock(block, variables, inFunc);
                prevCond = true;
            } else{
                prevCond = false;
            } 
            i++;
        } 

        else if (tokens[i].token == "def") {
		i++;
		std::string funcName = "";
		while(tokens[i].token != "(") {
			funcName += tokens[i].token;
			i++;
		}
		i++;
            std::vector<Token> arguments;
            bool argIndex = true;
            while (tokens[i].token != ")") {
                if(argIndex){
                    arguments.push_back(tokens[i]);
                    argIndex = false;
                } if(tokens[i].token == ","){
                    argIndex = true;
                }
                i++;
            }
		
		i++;
            
            int blockStart = i + 1;
            int blockParen = 1;
            while (blockParen > 0) {
                i++;
                if (tokens[i].token == "{") blockParen++;
                else if (tokens[i].token == "}") blockParen--;
            }
            
            
            std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i); 
 		          
            variables[funcName] = std::make_shared<Function>(Function{arguments, block, variables});

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

            if(prevCond == false && (isBool(evaluateExpression(conditionExpr, variables)) == true)){
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i);
                parseBlock(block, variables, inFunc);
                prevCond = true;
            }
            i++;
        }
        
        else if (tokens[i].token == "else") {

	    i++; 
            int blockStart = i + 1;


	    if(tokens[blockStart].token == "}"){
		i++;
            } else {

            int blockParen = 1;
          
            while (blockParen > 0) {
                i++;
                if (tokens[i].token == "{") blockParen++;
                else if (tokens[i].token == "}") blockParen--;
            }
            
            if(prevCond == false){
                std::vector<Token> block(tokens.begin() + blockStart, tokens.begin() + i);
                parseBlock(block, variables, inFunc);
            }
	  }
            prevCond = true;
        } 
        
        else if (tokens[i].token == "}"){
            i++;
        } 
        
        else if(tokens[i].type == END){
            return nullptr;
        }

        //Evalutate expressions that are not commands (And create vars)
        else {
            int blockStart = i;
            
            while(i < (int)tokens.size() && tokens[i].type != COMMAND && tokens[i].type != FUNCTION && tokens[i].token != "}"){
                i++;
            }
            int blockEnd = i;
            
            std::vector<Token> block (tokens.begin() + blockStart, tokens.begin() + blockEnd);
            
            if((int)block.size() > 0 && block.at(0).type != END){
                evaluateExpressionChunk(block, variables);
            }
            
        }
    }

    return nullptr;
}


