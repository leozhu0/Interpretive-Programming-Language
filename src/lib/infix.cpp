#include "infix.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <cmath>
#include <iomanip>

//std::map<std::string, Value> variables;
//std::map<std::string, bool> isBool;

InfixParser::InfixParser(std::vector<Token> tokens, std::map<std::string, Value>& variables) : varCache(variables) {
  if (tokens.size() == 1) {
    std::ostringstream error;
    error << "1 Unexpected token at line " << tokens[0].line << " column " << tokens[0].column << ": " << tokens[0].token;
    throw std::runtime_error(error.str());
  }

  if (tokens[0].type == OPERATOR || tokens[0].type == ASSIGNMENT || tokens[0].token == ")") {
    std::ostringstream error;
    error << "2 Unexpected token at line " << tokens[0].line << " column " << tokens[0].column << ": " << tokens[0].token;
    throw std::runtime_error(error.str());
  }

  // creating the tree starts here
  // nextNode is called to get the first token
  // uses precedence of 0 as a base condition
  root = createTree(nextNode(tokens), 0, tokens);

  if (parenNum != 0) {
    delete root;
    std::ostringstream error;
    error << "3 Unexpected token at line " << tokens[tokens.size() - 1].line << " column " << tokens[tokens.size() - 1].column << ": " << tokens[tokens.size() - 1].token;
    throw std::runtime_error(error.str());
  }

  if (updateVariables) {
    for (const auto& pair : variableBuffer) {
      variables[pair.first] = pair.second->getValue(variables);
      //isBool[pair.first] = (pair.second->returnType == BOOL ? true : false);
    }
  }
  
  index = 0;
  updateVariables = true;
}

InfixParser::~InfixParser() {
  delete root;
}

// creates the actual tree from the given vector of tokens
// look at the precedence helper function to see the precedence of each operator
// several try-catch to prevent memory leaks
// read within the try statements for the core lines
// operator precedence parsing: https://en.wikipedia.org/wiki/Operator-precedence_parser#Pseudocode
Node* InfixParser::createTree(Node* leftHandSide, int minPrecedence, std::vector<Token> tokens) {
  std::string nextOp;

  // gets the first operator
  try {
    nextOp = peak(tokens).token;
  }
  catch (const std::exception& e){
    delete leftHandSide;
    throw std::runtime_error(e.what());
  }

  // goes through the vector until an operator of lower precedence is reached
  while (precedence(nextOp) >= minPrecedence) {
    std::string currOp = nextOp;
    Node* rightHandSide;

    try {
      rightHandSide = nextNode(tokens);
    }
    catch (const std::exception& e) {
      delete leftHandSide;
      throw std::runtime_error(e.what());
    }

    try {
      nextOp = peak(tokens).token;
    }
    catch (const std::exception& e) {
      delete leftHandSide;
      delete rightHandSide;
      throw std::runtime_error(e.what());
    }

    // this while loop deals with higher precedence operators to the right through recursion
    // second part of the or statement and the addedPrecedence variable are to parse assignment which is righ associative
    while ((precedence(nextOp) > precedence(currOp)) || (nextOp == "=" && precedence(nextOp) == precedence(currOp))) {
      int addedPrecedence = 1;
      if (nextOp == "=") --addedPrecedence;
      size_t parenNumBuffer = parenNum;

      try {
        rightHandSide = createTree(rightHandSide, precedence(currOp) + addedPrecedence, tokens);
      }
      catch (const std::exception& e) {
	delete leftHandSide;
	throw std::runtime_error(e.what());
      }

      parenNum = parenNumBuffer;

      try {      
	nextOp = peak(tokens).token;
      }
      catch (const std::exception& e) {
        delete leftHandSide;
        delete rightHandSide;
        throw std::runtime_error(e.what());
      }
    }

    // below connects the parts of tree
    OpNode* tempNode;

    if (currOp == "=") tempNode = new AssignNode;
    else if (currOp == "==" || currOp == "!=" || currOp == ">" || currOp == "<" || currOp == ">=" || currOp == "<=") tempNode = new CompareNode;
    else if (currOp == "|" || currOp == "^" || currOp == "&") tempNode = new LogicNode;
    else tempNode = new OpNode;

    tempNode->value = currOp;
    tempNode->lhs = leftHandSide;
    tempNode->rhs = rightHandSide;

    if (currOp == "=") {
      variableBuffer.push_back({leftHandSide->toString(), rightHandSide});
      tempNode->returnType = rightHandSide->returnType;
      leftHandSide->returnType = rightHandSide->returnType;
    }

    leftHandSide = tempNode;
  }

  return leftHandSide;
}

// helper function to retrieve precedence
int InfixParser::precedence(std::string op) {
  if (op == "=") return 0;

  else if (op == "|") return 1;

  else if (op == "^") return 2;

  else if (op == "&") return 3;

  else if (op == "==" || op == "!=") return 4;

  else if (op == "<" || op == ">" || op == "<=" || op == ">=") return 5;

  else if (op == "+" || op == "-") return 6;

  else if (op == "*" || op == "/" || op == "%") return 7;

  else if (op == "END" || op == ")" || op == "," || op == "]") return -1;

  else {
    throw std::runtime_error("Undefined operator");
  }
}

// returns the next operator given the stored index member variable
// considers closing parenthesis and END as operators
// deals with errors
Token& InfixParser::peak(std::vector<Token> tokens) {
  for (size_t i = index + 1; i < tokens.size(); ++i) {
    if (tokens[i].type == OPERATOR || tokens[i].type == ASSIGNMENT || tokens[i].type == COMPARE || tokens[i].type == LOGIC || tokens[i].type == COMMA || tokens[i].token == "]") {

      if ((tokens[i - 1].token == "(") /*|| (tokens[i].type == ASSIGNMENT && tokens[i - 1].type != VARIABLE)*/) {
        std::ostringstream error;
        error << "AAA Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
	throw std::runtime_error(error.str());
      }

      else if (tokens[i + 1].type == OPERATOR || tokens[i + 1].type == ASSIGNMENT || tokens[i + 1].type == COMPARE || tokens[i + 1].type == LOGIC || tokens[i + 1].token == ")" || (tokens[i + 1].type == END && tokens[i].token != "]")) {
        std::ostringstream error;
        error << "BBB Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
	throw std::runtime_error(error.str());
      }

      return tokens[i];
    }

    else if (tokens[i].token == ")") {
      if (parenNum == 0) {
        std::ostringstream error;
        error << "CCC Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
	throw std::runtime_error(error.str());
      }

      if (tokens[i + 1].token == "(") {
        std::ostringstream error;
	error << "DDD Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
	throw std::runtime_error(error.str());
      }

      --parenNum;
      return tokens[i];
    }

  }

  // returns END token
  return tokens.back();
}

// returns the next number or variable given the stored index member variable
// calls createTree with base parameters when it hits an open parenthesis
// deals with errors
Node* InfixParser::nextNode(std::vector<Token> tokens) {
  for (size_t i = index + 1; i < tokens.size(); ++i) {
    if (tokens[i].type == NUMBER || tokens[i].type == BOOL) {

      if (tokens[i + 1].type == NUMBER || tokens[i + 1].type == VARIABLE || tokens[i + 1].type == BOOL || tokens[i + 1].token == "(") {
        std::ostringstream error;
        error << "EEE Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
	throw std::runtime_error(error.str());
      }

      else if (i != 0 && tokens[i - 1].token == ")") {
        std::ostringstream error;
        error << "FFF Unexpected token at line " << tokens[i - 1].line << " column " << tokens[i - 1].column << ": " << tokens[i - 1].token;
	throw std::runtime_error(error.str());
      }

      index = i;

      Node* tempNode;

      if (tokens[i].type == NUMBER) tempNode = new NumNode;
      else tempNode = new BoolNode;

      tempNode->value = stringToValue(tokens[i]);

      return tempNode;
    }

    else if (tokens[i].type == VARIABLE) {
      
      if (tokens[i + 1].type == NUMBER || tokens[i + 1].type == VARIABLE || tokens[i + 1].type == BOOL || tokens[i + 1].token == "(") {
        std::ostringstream error;
        error << "GGG Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
	throw std::runtime_error(error.str());
      }

      else if (i != 0 && tokens[i - 1].token == ")") {
        std::ostringstream error;
        error << "HHH Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
	throw std::runtime_error(error.str());
      }

      index = i;

      VarNode* tempNode = new VarNode;
      tempNode->value = tokens[i].token;

      // do not update stored variables when there is an error
      if (tokens[i + 1].token != "=") {
        try {
	  tempNode->getValue(varCache);
	}
	catch (const std::exception& e) {
	  updateVariables = false;
	}
      }

      return tempNode;
    }

    else if (tokens[i].token == "(") {
      if (tokens[i + 1].token == ")" || tokens[i + 1].type == OPERATOR || tokens[i + 1].type == ASSIGNMENT) {
        std::ostringstream error;
        error << "III Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
	throw std::runtime_error(error.str());
      }
      
      index = i;
      ++parenNum;

      // creates a tree for parenthesis case
      Node* tempNode = createTree(nextNode(tokens), 0, tokens);

      ++index;

      return tempNode;
    }

    else if (tokens[i].token == "[") {
      // add error cases from "("
      // definitely more error cases that could be added to this as well as "(" case (such as type == logic or comparison)
      //size_t bracketNum = 1;
      index = i;

      ArrayNode* tempNode = new ArrayNode;

      while (tokens[index + 1].token != "]") {
        tempNode->value.push_back(createTree(nextNode(tokens), 0, tokens));
        
	if (tokens[index + 1].token == "]") break;
	++index;
      }

      ++index;

      // Parsing array lookup
      if (tokens[index + 1].token == "[") {
        ++index;
	tempNode->lookUp = createTree(nextNode(tokens), 0, tokens);
	++index;
      }

      return tempNode;

      /*
      size_t bracketNum = 1;
      ++i;

      ArrayNode* tempNode = new ArrayNode;

      // add error cases where it reaches the end of the vector
      while (true) {
        std::vector<Token> tempTokens;

        while (true) {
          if (tokens[i].token == "[") ++bracketNum;
	  else if (tokens[i].token == "]") --bracketNum;

	  if (bracketNum == 0) break;
	  if (tokens[i].token == "," && bracketNum == 1) break;

	  tempTokens.push_back(tokens[i]);
	  ++i;
	}

	if (tempTokens.size() != 0) {
	  tempTokens.push_back(tokens[i]);
          tempTokens.back().token = "END";
	  tempTokens.back().type = END;

	  for (Token token : tempTokens) {
	    std::cout << token.token << " " << token.type << " " << " " << token.line << " " <<  token.column << std::endl;
	  }
	  std::cout << "_____" << std::endl;
          
	  tempNode->value.push_back(createTree(nextNode(tempTokens), 0, tempTokens));
	}

	if (bracketNum == 0) break;
	else ++i;
      }

      // Parsing array lookup
      if (tokens[i + 1].token == "[") {
        i += 2;
	std::vector<Token> tempTokens;

	while (tokens[i].token != "]") {
	  tempTokens.push_back(tokens[i]);
	  ++i;
	}

	tempTokens.push_back(tokens[i]);
        tempTokens.back().token = "END";
        tempTokens.back().type = END;

	tempNode->lookUp = createTree(nextNode(tempTokens), 0, tempTokens);
      }

      std::cout << "working" << std::endl;

      index = i;
      return tempNode;
      */
    }

    else if (tokens[i].type == COMMAND) {
      std::ostringstream error;
      error << "JJJ Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
      throw std::runtime_error(error.str());
    }

  }

  std::ostringstream error;
  error << "LAST Unexpected token at line " << tokens[tokens.size() - 1].line << " column " << tokens[tokens.size() - 1].column << ": " << tokens[tokens.size() - 1].token;
  throw std::runtime_error(error.str());
}

Value InfixParser::stringToValue(Token& token) {
  Value result;

  if (token.type == BOOL) {
    if (token.token == "true") result = true;
    else result = false;

  } else if (token.type == NUMBER) {
    result = std::stod(token.token);
  }

  return result;
}

std::string InfixParser::toString() {
  return root->toString();
}

Value InfixParser::calculate() {
  return root->getValue(varCache);
}

//___________________________________________________________________________________________________
TokenType Node::getReturnType([[maybe_unused]] std::map<std::string, Value>& variables) {
  return returnType;
}

Value NumNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  return value;
}

std::string NumNode::toString() {
  std::string result = std::to_string(std::get<double>(value));
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

Value VarNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  if(variables.find(value) == variables.end()){
    std::ostringstream error;
    error <<"Runtime error: unknown identifier " << value;
    throw std::runtime_error(error.str());
  }

  returnType = (std::holds_alternative<bool>(variables[value]) ? BOOL : NUMBER);
  
  return variables[value];
}

std::string VarNode::toString() {
  return value;
}

TokenType VarNode::getReturnType([[maybe_unused]] std::map<std::string, Value>& variables) {
  if (std::holds_alternative<bool>(variables[value])) return BOOL;
  else return NUMBER;
}

Value BoolNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  return value;
}

std::string BoolNode::toString() {
  if (std::get<bool>(value)) return "true";
  else return "false";
}

Value ArrayNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  Value result;

  if (lookUp == nullptr) {
    result = std::make_shared<std::vector<Value>>();

    for (Node* node : value) {
      std::get<Array>(result)->push_back(node->getValue(variables));
    }
  }

  else {
    result = value[std::get<double>(lookUp->getValue(variables))]->getValue(variables);
  } 

  return result;
}

std::string ArrayNode::toString() {
  std::ostringstream result;
  result << "[";

  for (Node* node : value) {
    result << node->toString();

    if (node != value.back()) result << ", ";
  }

  result << "]";

  if (lookUp != nullptr) {
    result << "[" << lookUp->toString() << "]";
  }

  return result.str();
}

OpNode::~OpNode() {
  delete lhs;
  delete rhs;
}

Value OpNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  //if (lhs->getReturnType(variables) != NUMBER || rhs->getReturnType(variables) != NUMBER) {

  if (!(std::holds_alternative<double>(lhs->getValue(variables)) && std::holds_alternative<double>(rhs->getValue(variables)))) {
    std::ostringstream error;
    error << "Runtime error: invalid operand type.";
    throw std::runtime_error(error.str());
  }

  if (value == "+") return std::get<double>(lhs->getValue(variables)) + std::get<double>(rhs->getValue(variables));

  else if (value == "-") return std::get<double>(lhs->getValue(variables)) - std::get<double>(rhs->getValue(variables));

  else if (value == "*") return std::get<double>(lhs->getValue(variables)) * std::get<double>(rhs->getValue(variables));

  else if (value == "/") {
    if (std::get<double>(rhs->getValue(variables)) == 0) {
      std::ostringstream error;
        error << "Runtime error: division by zero.";
        throw std::runtime_error(error.str());
      }

    return std::get<double>(lhs->getValue(variables)) / std::get<double>(rhs->getValue(variables));
  }

  else if (value == "%") return std::fmod(std::get<double>(lhs->getValue(variables)), std::get<double>(rhs->getValue(variables)));

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

Value AssignNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  if (!(lhs->isVar)) {
    std::ostringstream error;
    error << "Runtime error: invalid assignee.";
    throw std::runtime_error(error.str());
  }

  returnType = rhs->getReturnType(variables);
  return lhs->getValue(variables);
}

TokenType AssignNode::getReturnType([[maybe_unused]] std::map<std::string, Value>& variables) {
  return lhs->getReturnType(variables);
}

Value CompareNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  if ((value == "==" || value == "!=") && lhs->getReturnType(variables) != rhs->getReturnType(variables)) return false;

  if (std::holds_alternative<double>(lhs->getValue(variables))) {
    if (value == "==") return std::get<double>(lhs->getValue(variables)) == std::get<double>(rhs->getValue(variables));
    else if (value == "!=") return std::get<double>(lhs->getValue(variables)) != std::get<double>(rhs->getValue(variables));
  }

  else if (std::holds_alternative<bool>(lhs->getValue(variables))) {
    if (value == "==") return std::get<bool>(lhs->getValue(variables)) == std::get<bool>(rhs->getValue(variables));
    else if (value == "!=") return std::get<bool>(lhs->getValue(variables)) != std::get<bool>(rhs->getValue(variables));
  }

  if (lhs->getReturnType(variables) != rhs->getReturnType(variables) || lhs->getReturnType(variables) == BOOL) {
    std::ostringstream error;
    error << "Runtime error: invalid operand type.";
    throw std::runtime_error(error.str());
  }

  if (value == "<") return std::get<double>(lhs->getValue(variables)) < std::get<double>(rhs->getValue(variables));

  else if (value == ">") return std::get<double>(lhs->getValue(variables)) > std::get<double>(rhs->getValue(variables));

  else if (value == "<=") return std::get<double>(lhs->getValue(variables)) <= std::get<double>(rhs->getValue(variables));

  else if (value == ">=") return std::get<double>(lhs->getValue(variables)) >= std::get<double>(rhs->getValue(variables));

  else {
    std::cout << "This error should never happen. 1" << std::endl;
    exit(1);
  }
}

Value LogicNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  //if (lhs->getReturnType(variables) != BOOL || rhs->getReturnType(variables) != BOOL) {

  if (!(std::holds_alternative<bool>(lhs->getValue(variables)) && std::holds_alternative<bool>(rhs->getValue(variables)))) {
    std::ostringstream error;
    error << "Runtime error: invalid operand type.";
    throw std::runtime_error(error.str());
  }

  if (value == "&") return std::get<bool>(lhs->getValue(variables)) && std::get<bool>(rhs->getValue(variables));

  else if (value == "|") return std::get<bool>(lhs->getValue(variables)) || std::get<bool>(rhs->getValue(variables));

  else if (value == "^") return ((std::get<bool>(lhs->getValue(variables)) || std::get<bool>(rhs->getValue(variables))) && !(std::get<bool>(rhs->getValue(variables)) && std::get<bool>(rhs->getValue(variables))));

  else {
    std::cout << "This error should never happen. 2" << std::endl;
    exit(1);
  }
}

