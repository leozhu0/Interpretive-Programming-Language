#include "infix.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

InfixParser::InfixParser(std::vector<Token> tokens) {

  
  if (tokens[1].token == "=") {
  for (Token token : tokens) {
    std::cout << token.token << std::endl;
  }
  std::cout << "_______________________________________" << std::endl;
  }
  

  if (tokens.size() == 0) {
    throw std::runtime_error("No tokens");
  }

  if (tokens.size() == 1) {
    std::ostringstream error;
    error << "Unexpected token at line " << tokens[0].line << " column " << tokens[0].column << ": " << tokens[0].token;
    throw std::runtime_error(error.str());
  }

  if (tokens[0].type == OPERATOR || tokens[0].type == ASSIGNMENT || tokens[0].token == ")") {
    std::ostringstream error;
    error << "Unexpected token at line " << tokens[0].line << " column " << tokens[0].column << ": " << tokens[0].token;
    throw std::runtime_error(error.str());
  }

  // creating the tree
  root = createTree(nextNode(tokens), 0, tokens);

  if (parenNum != 0) {
    std::ostringstream error;
    error << "Unexpected token at line " << tokens[tokens.size() - 1].line << " column " << tokens[tokens.size() - 1].column << ": " << tokens[tokens.size() - 1].token;
    throw std::runtime_error(error.str());
  }

  // resetting the index after parsing through a list of tokens
  index = 0;
}

InfixParser::~InfixParser() {
  delete root;
}

Node* InfixParser::createTree(Node* leftHandSide, int minPrecedence, std::vector<Token> tokens) {
  std::string nextOp = peak(tokens).token;

  while (precedence(nextOp) >= minPrecedence) {
    std::string currOp = nextOp;
    
    Node* rightHandSide = nextNode(tokens);

    nextOp = peak(tokens).token;

    while ((precedence(nextOp) > precedence(currOp)) || (nextOp == "=" && precedence(nextOp) == precedence(currOp))) {
      int addedPrecedence = 1;
      if (nextOp == "=") --addedPrecedence;

      rightHandSide = createTree(rightHandSide, precedence(currOp) + addedPrecedence, tokens);
      nextOp = peak(tokens).token;
    }

    OpNode* tempNode;

    if (currOp == "=") tempNode = new AssignNode;
    else tempNode = new OpNode;

    tempNode->value = currOp;
    tempNode->children.push_back(leftHandSide);
    tempNode->children.push_back(rightHandSide);

    if (currOp == "=") variables[leftHandSide->toString()] = rightHandSide->getValue();

    leftHandSide = tempNode;
  }

  return leftHandSide;
}

int InfixParser::precedence(std::string op) {
  if (op == "=") return 0;

  else if (op == "+" || op == "-") return 1;

  else if (op == "*" || op == "/") return 2;

  else if (op == "END" || op == ")") return -1;

  else {
    throw std::runtime_error("Undefined operator");
  }
}

Token& InfixParser::peak(std::vector<Token> tokens) {
  for (size_t i = index + 1; i < tokens.size(); ++i) {
    if (tokens[i].type == OPERATOR || tokens[i].type == ASSIGNMENT) {

      if ((tokens[i - 1].token == "(") || (tokens[i].type == ASSIGNMENT && tokens[i - 1].type != VARIABLE)) {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
        throw std::runtime_error(error.str());
      }

      else if (tokens[i + 1].type == OPERATOR || tokens[i + 1].type == ASSIGNMENT || tokens[i + 1].token == ")" || tokens[i + 1].type == END) {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
        throw std::runtime_error(error.str());
      }

      return tokens[i];
    }

    else if (tokens[i].token == ")") {
      if (parenNum == 0) {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
        throw std::runtime_error(error.str());
      }

      if (tokens[i + 1].token == "(") {
        std::ostringstream error;
	error << "Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
	throw std::runtime_error(error.str());
      }

      --parenNum;
      //std::cout << "PN: " << parenNum << std::endl;
      return tokens[i];
    }

  }

  return tokens.back();
}

Node* InfixParser::nextNode(std::vector<Token> tokens) {
  for (size_t i = index + 1; i < tokens.size(); ++i) {
    if (tokens[i].type == NUMBER) {

      if (tokens[i + 1].type == NUMBER || tokens[i + 1].type == VARIABLE || tokens[i + 1].token == "(") {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
        throw std::runtime_error(error.str());
      }

      else if (i != 0 && tokens[i - 1].token == ")") {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i - 1].line << " column " << tokens[i - 1].column << ": " << tokens[i - 1].token;
        throw std::runtime_error(error.str());
      }

      index = i;

      NumNode* tempNode = new NumNode;
      tempNode->value = tokens[i].token;

      return tempNode;
    }

    else if (tokens[i].type == VARIABLE) {
      
      if (tokens[i + 1].type == NUMBER || tokens[i + 1].type == VARIABLE || tokens[i + 1].token == "(") {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
        throw std::runtime_error(error.str());
      }

      else if (i != 0 && tokens[i - 1].token == ")") {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
        throw std::runtime_error(error.str());
      }

      index = i;

      VarNode* tempNode = new VarNode;
      tempNode->value = tokens[i].token;

      return tempNode;
    }

    else if (tokens[i].token == "(") {
      if (tokens[i + 1].token == ")") {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
        throw std::runtime_error(error.str());
      }
      
      index = i;
      ++parenNum;
      //std::cout << "PN: " << parenNum << std::endl;

      Node* tempNode = createTree(nextNode(tokens), 0, tokens);

      ++index;
      /*
      if (tokens[index + 1].token != ")") {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[index + 1].line << " column " << tokens[index + 1].column << ": " << tokens[index + 1].token;
        throw std::runtime_error(error.str());
      }

      if (tokens[index + 2].token == "(") {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[index + 2].line << " column " << tokens[index + 2].column << ": " << tokens[index + 2].token;
        throw std::runtime_error(error.str());
      }
      */

      return tempNode;
    }

  }

  std::ostringstream error;
  error << "Unexpected token at line " << tokens[tokens.size() - 1].line << " column " << tokens[tokens.size() - 1].column << ": " << tokens[tokens.size() - 1].token;
  throw std::runtime_error(error.str());
}


std::string InfixParser::toString() {
  return root->toString();
}

double InfixParser::calculate() {
  return root->getValue();
}

