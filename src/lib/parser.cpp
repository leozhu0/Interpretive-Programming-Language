#include "parser.h"
#include <iostream>

Parser::~Parser() {
  delete root;
}

Parser::Parser(std::vector<Token> tokens) {
  if (tokens.size() == 0) {
    std::cout << "No tokens" << std::endl;
    exit(2);
  }

  root = createNode(tokens);
}

Node* Parser::createNode(std::vector<Token> tokens) {
  size_t start = 0;

  // If the expression is just a single number
  if (tokens[start].type == NUMBER) {
    if (tokens.size() > 2) {
      std::cout << "Unexpected token at line " << tokens[1].line << " column " << tokens[1].column << ": " << tokens[1].token << std::endl;
      exit(2);
    }

    NumNode* node = new NumNode;
    node->value = tokens[start].token;

    return node;

  // If the expression has an operator
  } else if (tokens[start].token == "(") {
    if (tokens[start + 1].type != OPERATOR) {
      std::cout << "Unexpected token at line " << tokens[start + 1].line << " column " << tokens[start + 1].column << ": " << tokens[start + 1].token << std::endl;
      exit(2);
    }

    ++start;
    // variable to check for parenthesis error
    size_t allowedParenthesis = 1;

    OpNode* node = new OpNode;
    node->value = tokens[start].token;

    // iterates through everything following the operator
    for (size_t i = start + 1; i < tokens.size(); ++i) {
      if (allowedParenthesis == 0 && i != tokens.size() - 1) {
        std::cout << "Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
	exit(2);
      }

      if (tokens[i].token == ")") {
	if (i == start + 1) {
          std::cout << "Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
	  exit(2);
	}

        --allowedParenthesis;
      }
      
      // adding a number to the child pointers
      else if (tokens[i].type == NUMBER) {
	NumNode* tempNode = new NumNode;
	tempNode->value = tokens[i].token;
        node->children.push_back(tempNode);
      }

      // adding an operator to the child pointers
      else if (tokens[i].token == "(") {
	size_t parenNum = 1;
	std::vector<Token> tempTokens;
	tempTokens.push_back(tokens[i]);
	++i;

	// creating a new vector to be called recursively
	while (true) {
	  if (tokens[i].token == "(") ++parenNum;
	  else if (tokens[i].token == ")") --parenNum;

          tempTokens.push_back(tokens[i]);

	  if (parenNum == 0) break;
	  else ++i;

	  if (i == tokens.size()) {  
            std::cout << "Unexpected token at line " << tokens[i - 1].line << " column " << tokens[i - 1].column << ": " << tokens[i - 1].token << std::endl;
	    exit(2);
	  }
	}

	tempTokens.push_back(tokens[tokens.size() - 1]);
	node->children.push_back(createNode(tempTokens));
      }

      else if (tokens[i].type == OPERATOR) {
        std::cout << "Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
	exit(2);
      }
    }

    if (allowedParenthesis != 0) {
      std::cout << "Unexpected token at line " << tokens[tokens.size() - 2].line << " column " << tokens[tokens.size() - 2].column + tokens[tokens.size() - 2].token.size() << ": " << tokens[tokens.size() - 1].token << std::endl;
      exit(2);
    }

    return node;

  // default error case
  } else {
    std::cout << "Unexpected token at line " << tokens[start].line << " column " << tokens[start].column << ": " << tokens[start].token << std::endl;
    exit(2);
  }

}

std::string Parser::toString() {
  return root->toString();
}

double Parser::calculate() {
  return root->getValue();
}

double NumNode::getValue() {
  return std::stod(value);
}

std::string NumNode::toString() {
  std::string result = value;
  bool hasDecimal = false;

  // removing trailing 0s after the decimal
  for (char digit : result) {
    if (digit == '.') {
      hasDecimal = true;
      break;
    }
  }

  while (hasDecimal == true && result.back() == '0') {
    result.pop_back();
  }

  if (result.back() == '.') result.pop_back();
      	    
  return result; 
}

OpNode::~OpNode() {
  for (Node* child : children) {
    delete child;
  }
}

double OpNode::getValue() {
  double result;

  // There is one case for each operator type
  if (value == "+") {
    result = 0;

    for (Node* child : children) {
      result += child->getValue();
    }

    return result;

  } else if (value == "-") {
    result = (2 * children[0]->getValue());

    for (Node* child : children) {
      result -= child->getValue();
    }

    return result;

  } else if (value == "*") {
    result = 1;

    for (Node* child : children) {
      result *= child->getValue();
    }

    return result;

  } else if (value == "/") {
    result = children[0]->getValue();
    
    for (size_t i = 1; i < children.size(); i++) {
      if (children[i]->getValue() == 0) {
	std::cout << "Runtime error: division by zero." << std::endl;
	exit(3);
      }

      result /= children[i]->getValue();
    }

    return result;

  } else {
    exit(2);
  }
}

std::string OpNode::toString() {
  std::string result = "(";
  
  for (Node* child : children) {
    result += child->toString();

    if (child != children[children.size() - 1]) {
      result += ' ';
      result += value;
      result += ' ';
    }
  }

  result += ')';
  return result;
}

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
}

InfixParser::~InfixParser() {
  delete root;
}

Node* InfixParser::createTree(Node* leftHandSide, int minPrecedence, std::vector<Token> tokens) {
  std::string nextOp = peak(tokens).token;

  while (precedence(nextOp) >= minPrecedence) {
    std::string currOp = nextOp;
    
    //TODO add variable case
    NumNode* rightHandSide = new NumNode;
    rightHandSide->value = nextNum(tokens).token;

    nextOp = peak(tokens).token;

    //TODO add equivalent case with right-associative operator
    while (precedence(nextOp) > precedence(currOp)) {
      rightHandSide = createTree(rightHandSide, precedence(currOp) + 1, tokens);
      nextOp = peak(tokens).token;
    }

    OpNode tempNode = new OpNode;
    tempNode->left = leftHandSide;
    tempNode->right = rightHandSide;

    leftHandSide = tempNode;
  }

  return leftHandSide;
}
