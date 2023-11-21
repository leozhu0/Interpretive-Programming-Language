#pragma once
#include "lexer.cpp" // cpp
#include "infix.cpp" //cpp
#include "value.cpp"
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
