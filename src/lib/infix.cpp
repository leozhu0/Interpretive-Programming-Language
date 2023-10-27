#include "infix.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

InfixParser::InfixParser(std::vector<Token> tokens) {
  if (tokens.size() == 0) {
    throw std::runtime_error("No tokens");
  }

  if (tokens.size() == 1) {
    std::ostringstream error;
    error << "Unexpected token at line " << tokens[0].line << " column " << tokens[0].column << ": " << tokens[0].token;
    throw std::runtime_error(error.str());
  }

  root = createTree(nextNode(tokens), 0, tokens);

  if (parenNum != 0) {
    std::ostringstream error;
    error << "Unexpected token at line " << tokens[tokens.size() - 1].line << " column " << tokens[tokens.size() - 1].column << ": " << tokens[tokens.size() - 1].token;
    throw std::runtime_error(error.str());
  }

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
    tempNode->children[0] = leftHandSide;
    tempNode->children[1] = rightHandSide;

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
  for (size_t i = index; i < tokens.size(); ++i) {
    if (tokens[i].type == OPERATOR) return tokens[i];

    else if (tokens[i].token == ")") {
      if (parenNum == 0) {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
        throw std::runtime_error(error.str());
      }

      --parenNum;
      return tokens[i];
    }

  }

  return tokens.back();
}

Node* InfixParser::nextNode(std::vector<Token> tokens) {
  for (size_t i = index + 1; i < tokens.size(); ++i) {
    if (tokens[i].type == NUMBER) {
      index = i;

      NumNode* tempNode = new NumNode;
      tempNode->value = tokens[i].token;

      return tempNode;
    }

    else if (tokens[i].type == VARIABLE) {
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
      Node* tempNode = createTree(nextNode(tokens), 0, tokens);

      if (tokens[index + 1].token != ")") {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[index + 1].line << " column " << tokens[index + 1].column << ": " << tokens[index + 1].token;
        throw std::runtime_error(error.str());
      }

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

