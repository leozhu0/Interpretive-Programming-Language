#include "parser.h"
#include <iostream>

Node::~Node() {
}

Parser::~Parser() {
  delete root;
}

Parser::Parser(std::vector<Token> tokens) {
  for (token& token : tokens) {
    std::cout << token.token << std::endl;
  }

  if (tokens.size() == 0) {
    std::cout << "No tokens" << std::endl;
    exit(2);
  }

  for (Token& token : tokens) {
    if (token.type == OPERATOR || token.type == NUMBER) {
      root = createNode(tokens);
      return;
    }
  }

  std::cout << "No valid tokens" << std::endl;
  exit(2);
}

Node* Parser::createNode(std::vector<Token> tokens) {
  if (tokens[0].type == NUMBER) {
    NumNode* node = new NumNode;
    node->value = tokens[0].token;

    return node;

  } else if (tokens[0].type == OPERATOR) {
    OpNode* node = new OpNode;
    node->value = tokens[0].token;

    for (size_t i = 1; i < tokens.size(); ++ i) {
      if (tokens[i].type == NUMBER) node->children.push_back(createNode({tokens[i]}));

      else if (tokens[i].token == "(") {
	int parenNum = 1;
	std::vector<Token> tempTokens;
	++i;

	while (true) {
	  if (tokens[i].token == "(") parenNum++;
	  else if (tokens[i].token == ")") parenNum--;

	  if (parenNum == 0) break;

	  else {
	    tempTokens.push_back(tokens[i]);
	    ++i;
	  }
	}

	++i;
	node->children.push_back(createNode(tempTokens));
      }

    }

    return node;

  } else {
    std::cout << "Syntax error on line " << tokens[0].line << " column " << tokens[0].column << "." << "parser";
    exit(1);
  }

}

std::string Parser::toString() {
  return root->toString();
}

double Parser::calculate() {
  return root->getValue();
}

double Node::getValue() {
  return 0;
}

std::string  Node::toString() {
  return "If you see this, you messed up";
}

double NumNode::getValue() {
  return std::stod(value); 
}
    
std::string NumNode::toString() {
  return value;
}

OpNode::~OpNode() {
  for (Node* child : children) {
    delete child;
  }
}

double OpNode::getValue() {
  double result;

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
	std::cout << "Runtime error: division by zero.";
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
