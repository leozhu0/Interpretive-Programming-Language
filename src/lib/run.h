#ifndef RUN_H
#define RUN_H
#include "lexer.h" 
#include "infix.h" 
#include "value.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>


class Scrypt {
	private:
		void printV(std::vector<Token> tokens);
		Value evaluateExpression(std::vector<Token> tokens, std::map<std::string, Value>& variables);
		void evaluateExpressionChunk(std::vector<Token> tokens, std::map<std::string, Value>& variables);
		bool isKeyword(Token token);
		bool isBool(Value value);
	public:
		void parseBlock(std::vector<Token>& tokens, std::map<std::string, Value>& variables, bool inFunc);
};
#endif
