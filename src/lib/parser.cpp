#include "parser.h"
#include "token.h"

int Node::getValue() {
  return 0;
}

std::string Node::toString() {
  return "If you see this, you messed up";
}

double NumNode::getValue() {
  return std::stod(value); 
}
    
std::string NumNode::toString() {
  return value;
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
        cout << "Runtime error: division by zero.";
	exit(3);
      }

      result /= children[i]->getValue();
    }

    return result;

  } else {
    exit();
  }
}

std::string OpNode::toString() {
  string result = "(";
  
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
