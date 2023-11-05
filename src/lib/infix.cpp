#include "infix.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

InfixParser::InfixParser(std::vector<Token> tokens) {

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

  // creating the tree starts here
  // nextNode is called to get the first token
  // uses precedence of 0 as a base condition
  root = createTree(nextNode(tokens), 0, tokens);

  if (parenNum != 0) {
    delete root;
    std::ostringstream error;
    error << "Unexpected token at line " << tokens[tokens.size() - 1].line << " column " << tokens[tokens.size() - 1].column << ": " << tokens[tokens.size() - 1].token;
    throw std::runtime_error(error.str());
  }

  if (updateVariables) {
    for (const auto& pair : variableBuffer) {
      variables[pair.first] = pair.second->getValue();
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

      try {
        rightHandSide = createTree(rightHandSide, precedence(currOp) + addedPrecedence, tokens);
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

    if (currOp == "=") variableBuffer.push_back({leftHandSide->toString(), rightHandSide});

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

  else if (op == "END" || op == ")") return -1;

  else {
    throw std::runtime_error("Undefined operator");
  }
}

// returns the next operator given the stored index member variable
// considers closing parenthesis and END as operators
// deals with errors
Token& InfixParser::peak(std::vector<Token> tokens) {
  for (size_t i = index + 1; i < tokens.size(); ++i) {
    if (tokens[i].type == OPERATOR || tokens[i].type == ASSIGNMENT || tokens[i].type == COMPARE || tokens[i].type == LOGIC) {

      if ((tokens[i - 1].token == "(") || (tokens[i].type == ASSIGNMENT && tokens[i - 1].type != VARIABLE)) {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
	throw std::runtime_error(error.str());
      }

      else if (tokens[i + 1].type == OPERATOR || tokens[i + 1].type == ASSIGNMENT || tokens[i + 1].type == COMPARE || tokens[i + 1].type == LOGIC || tokens[i + 1].token == ")" || tokens[i + 1].type == END) {
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
        error << "Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
	throw std::runtime_error(error.str());
      }

      else if (i != 0 && tokens[i - 1].token == ")") {
        std::ostringstream error;
        error << "Unexpected token at line " << tokens[i - 1].line << " column " << tokens[i - 1].column << ": " << tokens[i - 1].token;
	throw std::runtime_error(error.str());
      }

      index = i;

      Node* tempNode;

      if (tokens[i].type == NUMBER) tempNode = new NumNode;
      else tempNode = new BoolNode;

      tempNode->value = tokens[i].token;

      return tempNode;
    }

    else if (tokens[i].type == VARIABLE) {
      
      if (tokens[i + 1].type == NUMBER || tokens[i + 1].type == VARIABLE || tokens[i + 1].type == BOOL || tokens[i + 1].token == "(") {
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

      // do not update stored variables when there is an error
      if (tokens[i + 1].token != "=") {
        try {
	  tempNode->getValue();
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
        error << "Unexpected token at line " << tokens[i + 1].line << " column " << tokens[i + 1].column << ": " << tokens[i + 1].token;
	throw std::runtime_error(error.str());
      }
      
      index = i;
      ++parenNum;

      // creates a tree for parenthesis case
      Node* tempNode = createTree(nextNode(tokens), 0, tokens);

      ++index;

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

