#include "parser.h"
#include <iostream>

Node::~Node() {
}

Parser::~Parser() {
  delete root;
}

Parser::Parser(std::vector<Token> tokens) {
  //if (tokens.size() > 25) {
  //  for (Token& token : tokens) {
  //    std::cout << token.token << std::endl;
  //  }
  //  std::cout << "_______________" << std::endl;
  //}

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

  std::cout << "Unexpected token at line " << tokens[0].line << " column " << tokens[0].column << ": " << tokens[0].token << std::endl;
  exit(2);
}

Node* Parser::createNode(std::vector<Token> tokens) {
  size_t start = 0;

  while (tokens[start].token == "(") {
    ++start;
  } 

  if (tokens[start].type == NUMBER) {
    if (tokens.size() >= 3) {
      std::cout << "Unxpected token at line " << tokens[1].line << " column " << tokens[0].column << ": " << tokens[1].token << std::endl;
      exit(2);
    }

    NumNode* node = new NumNode;
    node->value = tokens[start].token;

    return node;

  } else if (tokens[start].type == OPERATOR) {
    OpNode* node = new OpNode;
    node->value = tokens[start].token;

    for (size_t i = start + 1; i < tokens.size(); ++i) {
      if (tokens[i].type == NUMBER) node->children.push_back(createNode({tokens[i]}));

      else if (tokens[i].token == "(") {
	int parenNum = 1;
	std::vector<Token> tempTokens;
	++i;

	while (true) {
	  if (tokens[i].token == "(") ++parenNum;
	  else if (tokens[i].token == ")") --parenNum;

	  if (parenNum == 0) break;

	  else {
	    tempTokens.push_back(tokens[i]);
	    ++i;
	  }
	}

	node->children.push_back(createNode(tempTokens));
      }

      else if (i == start + 1 && tokens[i].token == ")") exit(2);
    }

    return node;

  } else {
    std::cout << "Syntax error on line " << tokens[0].line << " column " << tokens[0].column << ".";
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
  std::string result = value;
  bool hasDecimal = false;

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
      	    
  return std::stod(result); 
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
