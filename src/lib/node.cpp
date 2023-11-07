#include "node.h"
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <cmath>

/*
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

double VarNode::getValue(){
  if(variables.find(value) == variables.end()){
    std::ostringstream error;
    error <<"Runtime error: unknown identifier " << value;
    throw std::runtime_error(error.str());
  }

   return variables[value];
}

std::string VarNode::toString() {
   return value;
}

double BoolNode::getValue() {
  if (value == "true") return 1;
  return 0;
}

std::string BoolNode::toString() {
  return value;
}

OpNode::~OpNode() {
  delete lhs;
  delete rhs;
}

double OpNode::getValue() {
  if (lhs->returnType != NUMBER && lhs->returnType != NUMBER) {
    std::ostringstream error;
    error << "Runtime error: invalid operand type.";
    throw std::runtime_error(error.str());
  }

  if (value == "+") return std::plus<double>()(lhs->getValue(),rhs->getValue());

  else if (value == "-") return std::minus<double>()(lhs->getValue(),rhs->getValue());

  else if (value == "*") return std::multiplies<double>()(lhs->getValue(),rhs->getValue());

  else if (value == "/") {
    if (rhs->getValue() == 0) {
      std::ostringstream error;
        error << "Runtime error: division by zero.";
        throw std::runtime_error(error.str());
      }

    return std::divides<double>()(lhs->getValue(),rhs->getValue());
  } 

  else if (value == "%") return std::fmod(lhs->getValue(),rhs->getValue());

  else {
    std::cout << "This error should never happen. 3" << std::endl;
    exit(1);
  }
}

std::string OpNode::toString() {
  std::ostringstream result;
  result << "(" << lhs->toString() << " " << value << " " << rhs->toString() << ")";
  return result.str();
}

double AssignNode::getValue() {
  return lhs->getValue();
}

double CompareNode::getValue() {
  if (lhs->returnType != rhs->returnType) {
    std::ostringstream error;
    error << "Runtime error: invalid operand type.";
    throw std::runtime_error(error.str());
  }

  if (value == "==") return std::equal_to<double>()(lhs->getValue(),rhs->getValue());

  else if (value == "!=") return std::not_equal_to<double>()(lhs->getValue(),rhs->getValue());

  if (lhs->returnType == BOOL) {
    std::ostringstream error;
    error << "Runtime error: invalid operand type.";
    throw std::runtime_error(error.str());
  }

  if (value == "<") return std::less<double>()(lhs->getValue(),rhs->getValue());
  
  else if (value == ">") return std::greater<double>()(lhs->getValue(),rhs->getValue());

  else if (value == "<=") return std::less_equal<double>()(lhs->getValue(),rhs->getValue());

  else if (value == ">=") return std::greater_equal<double>()(lhs->getValue(),rhs->getValue());

  else {
    std::cout << "This error should never happen. 1" << std::endl;
    exit(1);
  }
}

double LogicNode::getValue() {
  if (lhs->returnType != BOOL && lhs->returnType != BOOL) {
    std::ostringstream error;
    error << "Runtime error: invalid operand type.";
    throw std::runtime_error(error.str());
  }

  if (value == "&") return std::logical_and<double>()(lhs->getValue(),rhs->getValue());

  else if (value == "|") return std::logical_or<double>()(lhs->getValue(),rhs->getValue());

  else if (value == "^") return ((lhs->getValue() || rhs->getValue()) && !(rhs->getValue() && rhs->getValue()));

  else {
    std::cout << "This error should never happen. 2" << std::endl;
    exit(1);
  }
}
*/
