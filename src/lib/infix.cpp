#include "parser.h"
#include <iostream>

InfixParser::InfixParser(std::vector<Token> tokens) {
  if (tokens.size() == 0) {
    std::cout << "No tokens" << std::endl;
    exit(2);
  }

  if (tokens.size() == 1) {
    //TODO
    std::cout << "placeholder error" << std::endl;
    exit(2);
  }

  NumNode* leftHandSide = new NumNode;
  leftHandSide->value = tokens[0].token;

  root = createTree(leftHandSide, 0, tokens);
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

    //TODO add assignment case
    OpNode tempNode = new OpNode;
    tempNode.value = currOp;
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

  else if (op = "END" || op == ")") return -1;

  else {
    //TODO
    std::cout << "placeholder error 2" << std::endl;
    exit(2);
  }
}

Token& InfixParser::peak(std::vector<Token> tokens) {
  for (size_t i = index; i < tokens.size(); ++i) {
    if (tokens[i].type == OPERATOR || tokens[i].token == ")") return tokens[i];
  }

  return tokens.back();
}

Node* InfixParser::nextNode(std::vector<Token> tokens) {
  for (size_t i = index + 1; i < tokens.size(); ++i) {
    if (tokens[i].type == NUMBER {
      index = i;

      NumNode tempNode = new NumNode;
      tempNode.value = token[i].token;

      return tempNode;
    }

    //TODO add variable case

    else if (token[i].token == "(") {
      index = i;
      Node* tempNode = createTree();

      if (token[index + 1].token != ")") {
        std::cout << "placeholder error 4" << std:endl;
	exit(2);
      }

      return tempNode;
    }

  }

  //TODO
  std::cout << "placeholder error 3" << std::endl;
  exit(2);
}


std::string InfixParser::toString() {
  return root->toString();
}

double InfixParser::calculate() {
  return root->getValue();
}

