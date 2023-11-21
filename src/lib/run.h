//#pragma once
#ifndef RUN_H
#define RUN_H
#include "lexer.h" // cpp
#include "infix.h" //cpp
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
	public:
		Value parseBlock(std::vector<Token>& tokens, std::map<std::string, Value> variables);
};
#endif
