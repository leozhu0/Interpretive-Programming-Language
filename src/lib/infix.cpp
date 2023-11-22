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
  std::cout << "__begin__" << std::endl;
  for (Token token : tokens) {
    std::cout << token.token;
  }
  std::cout << std::endl << "__end__" << std::endl;

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

  /*
  if (updateVariables) {
    for (const auto& pair : variableBuffer) {
      if (!(pair.first->isVar)) continue;

      Node* key = pair.first;
      Node* data = pair.second;

      VarNode* varKey = (VarNode*) key;
      if (varKey->arguments.size() != 0 || varKey->noArgs) continue;

      std::string keyStr = pair.first->toString();
      if (keyStr.back() == ']') {
        size_t openIndex = keyStr.find('[');
	size_t closeIndex = keyStr.size() - 1;
	keyStr.erase(openIndex, closeIndex - openIndex + 1);
      }

      // checking if the variable is already defined
      if (variables.find(keyStr) != variables.end()) {
	// if there is no look up, we can treat the variable value normally
        if (key->lookUp != nullptr) {
          // checking if the variable is an array, and if so, we only want to change the value of a specific index
	  if (std::holds_alternative<Array>(variables[keyStr])) {
            if (!(std::holds_alternative<double>(key->lookUp->getValue(variables)))) throw std::runtime_error("Runtime error: index is not a number.");

            double arrayIndex = std::get<double>(key->lookUp->getValue(variables));
	    Array tempArray = std::get<Array>(variables[keyStr]);

            if (std::fmod(arrayIndex, 1) != 0) throw std::runtime_error("Runtime error: index is not an integer.");
            if (arrayIndex >= tempArray->size() || arrayIndex < 0) throw std::runtime_error("Runtime error: index out of bounds.");

	    tempArray->at(arrayIndex) = data->getValue(variables);
	    continue;
	  }

          else throw std::runtime_error("Runtime error: not an array.");
	}
      }

      else if (key->lookUp != nullptr) throw std::runtime_error("Runtime error: not an array.");

      std::streambuf* coutBuffer = std::cout.rdbuf();
      std::stringstream tempStream;
      std::cout.rdbuf(tempStream.rdbuf());

      try {
	//std::cout << "hi" << std::endl;
	data->getValue(variables);
        variables[keyStr] = data->getValue(variables);
	//std::cout << "bye" << std::endl;
      }

      catch (...) {
	std::cout.rdbuf(coutBuffer);
        continue;
      }

      std::cout.rdbuf(coutBuffer);
      //isBool[pair.first] = (pair.second->returnType == BOOL ? true : false);
    }
  }
  */
  
  index = 0;
  //updateVariables = true;
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
      variableBuffer.push_back({leftHandSide, rightHandSide});
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
    if (tokens[i].type == OPERATOR || tokens[i].type == ASSIGNMENT || tokens[i].type == COMPARE || tokens[i].type == LOGIC || tokens[i].type == COMMA) {

      if ((tokens[i - 1].token == "(") /*|| (tokens[i].type == ASSIGNMENT && tokens[i - 1].type != VARIABLE)*/) {
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

    else if (tokens[i].token == "]") {
      // assuming valid pairing

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
    if (tokens[i].type == NUMBER || tokens[i].type == BOOL || tokens[i].type == NILL) {

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
      else if (tokens[i].type == BOOL) tempNode = new BoolNode;
      else tempNode = new NullNode;

      tempNode->value = stringToValue(tokens[i]);

      if (tokens[i + 1].token == "[") {
        ++index;
        tempNode->lookUp = createTree(nextNode(tokens), 0, tokens);
        ++index;
      }

      return tempNode;
    }

    else if (tokens[i].type == VARIABLE) {
      
      if (tokens[i + 1].type == NUMBER || tokens[i + 1].type == VARIABLE || tokens[i + 1].type == BOOL /*|| tokens[i + 1].token == "("*/) {
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

      // Accounting for array lookup
      if (tokens[index + 1].token == "[") {
        ++index;
        tempNode->lookUp = createTree(nextNode(tokens), 0, tokens);
        ++index;
      }

      // Accounting for function arguments
      else if (tokens[index + 1].token == "(") {
        ++index;
	tempNode->isVar = false;

        if (tokens[index + 1].token == ")") {
          tempNode->noArgs = true;
	  ++index;
	}
        
        else {
          size_t argParenNum = 1;
          size_t j = index + 1;
	  size_t bracketNum = 0;

	  while (true) {
	    if (tokens[j].token == "(") ++argParenNum;
	    else if (tokens[j].token == ")") --argParenNum;
	    else if (tokens[j].token == "[") ++bracketNum;
	    else if (tokens[j].token == "]") --bracketNum;

            if (argParenNum == 0) break;
	    else if (tokens[j].type == COMMA && (argParenNum == 1 && bracketNum == 0)) tempNode->arguments.push_back(createTree(nextNode(tokens), 0, tokens));
            // may need to increment index by one after comma case is called; currently unsure

            ++j;
	  }
	  
	  ++parenNum;
          tempNode->arguments.push_back(createTree(nextNode(tokens), 0, tokens));
	  ++index;
	}

      }

      // do not update stored variables when there is an error
      if (tokens[i + 1].token != "=" /*&& !(std::holds_alternative<Func>(variables[tempNode->value]))*/) {
        std::streambuf* coutBuffer = std::cout.rdbuf();

        try {
	  std::stringstream tempStream;
	  std::cout.rdbuf(tempStream.rdbuf());

	  tempNode->getValue(varCache);
	}
	catch (const std::exception& e) {
	  updateVariables = false;
	  std::cout.rdbuf(coutBuffer);
	}

	std::cout.rdbuf(coutBuffer);
      }

      return tempNode;
    }

    else if (tokens[i].token == "(") {
      if (tokens[i + 1].token == ")" || tokens[i + 1].type == OPERATOR || tokens[i + 1].type == ASSIGNMENT || tokens[i + 1].type == COMMA) {
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

    else if (tokens[i].token == "[") {
      // add error cases from "("
      // definitely more error cases that could be added to this as well as "(" case (such as type == logic or comparison)
      index = i;

      ArrayNode* tempNode = new ArrayNode;

      size_t bracketNum = 1;
      int j = index + 1;
      size_t innerParenNum = 0;

      while (true) {
        if (tokens[j].token == "[") ++bracketNum;
	else if (tokens[j].token == "]") --bracketNum;
        else if (tokens[j].token == "(") ++innerParenNum;
	else if (tokens[j].token == ")") --innerParenNum;

	if (bracketNum == 0) break;
	else if (tokens[j].type == COMMA && (bracketNum == 1 && innerParenNum == 0)) tempNode->value.push_back(createTree(nextNode(tokens), 0, tokens));

	++j;
      }

      if (j != index + 1) tempNode->value.push_back(createTree(nextNode(tokens), 0, tokens));

      /*
      while (tokens[index + 1].token != "]") {
        tempNode->value.push_back(createTree(nextNode(tokens), 0, tokens));
        
	if (tokens[index + 1].token == "]") break;
	++index;
      }
      */

      ++index;

      // Parsing array lookup
      if (tokens[index + 1].token == "[") {
        ++index;
	tempNode->lookUp = createTree(nextNode(tokens), 0, tokens);
	++index;

	//if (tokens[index + 1].token == "=") tempNode->isValidArrayAssignment = true;
      }

      return tempNode;
    }

    else if (tokens[i].type == COMMAND) {
      std::ostringstream error;
      error << "Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token;
      throw std::runtime_error(error.str());
    }

  }

  std::ostringstream error;
  error << "Unexpected token at line " << tokens[tokens.size() - 1].line << " column " << tokens[tokens.size() - 1].column << ": " << tokens[tokens.size() - 1].token;
  throw std::runtime_error(error.str());
}

Value InfixParser::stringToValue(Token& token) {
  Value result;

  if (token.type == BOOL) {
    if (token.token == "true") result = true;
    else result = false;
  }
  
  else if (token.type == NUMBER) {
    result = std::stod(token.token);
  }

  
  else if (token.type == NILL) {
    result = nullptr;
  }
  

  return result;
}

std::string InfixParser::toString() {
  return root->toString();
}

Value InfixParser::calculate() {
  if (updateVariables) {
    for (const auto& pair : variableBuffer) {
      if (!(pair.first->isVar)) continue;

      VarNode* key = (VarNode*) pair.first;
      Node* data = pair.second;

      if (key->arguments.size() != 0 || key->noArgs) continue;

      // if the variable is already defined and has lookup
      if (varCache.find(key->value) != varCache.end() && key->lookUp != nullptr) {
        // checking if the variable is an array, and if so, we only want to change the value of a specific index
        if (std::holds_alternative<Array>(varCache[key->value])) {
          if (!(std::holds_alternative<double>(key->lookUp->getValue(varCache)))) throw std::runtime_error("Runtime error: index is not a number.");

          double arrayIndex = std::get<double>(key->lookUp->getValue(varCache));
          Array tempArray = std::get<Array>(varCache[key->value]);

          if (std::fmod(arrayIndex, 1) != 0) throw std::runtime_error("Runtime error: index is not an integer.");
          if (arrayIndex >= tempArray->size() || arrayIndex < 0) throw std::runtime_error("Runtime error: index out of bounds.");

          tempArray->at(arrayIndex) = data->getValue(varCache);
          continue;
        }

        else throw std::runtime_error("Runtime error: not an array.");
      }

      // if variable is not defined and has lookup
      else if (varCache.find(key->value) == varCache.end() && key->lookUp != nullptr) throw std::runtime_error("Runtime error: not an array.");

      std::streambuf* coutBuffer = std::cout.rdbuf();
      std::stringstream tempStream;
      std::cout.rdbuf(tempStream.rdbuf());

      try {
        data->getValue(varCache);
        varCache[key->value] = data->getValue(varCache);
      } catch (...) {
        std::cout.rdbuf(coutBuffer);
        continue;
      }

      std::cout.rdbuf(coutBuffer);
    }

  return root->getValue(varCache);
}

//___________________________________________________________________________________________________
Node::~Node() {
  if (lookUp != nullptr) delete lookUp;
}

TokenType Node::getReturnType([[maybe_unused]] std::map<std::string, Value>& variables) {
  return returnType;
}

Value NumNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  if (lookUp != nullptr) throw std::runtime_error("Runtime error: not an array.");

  return value;
}

std::string NumNode::toString() {
  std::ostringstream result;

  result << std::get<double>(value);

  /*
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
  */

  if (lookUp != nullptr) result << "[" << lookUp->toString() << "]";

  return result.str();
}

VarNode::~VarNode() {
  if (arguments.size() == 0) return;

  for (Node* node : arguments) {
    delete node;
  }
}

Value VarNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  if(variables.find(value) == variables.end()){
    std::ostringstream error;
    error <<"Runtime error: unknown identifier " << value;
    throw std::runtime_error(error.str());
  }

  //returnType = (std::holds_alternative<bool>(variables[value]) ? BOOL : NUMBER);

  Value varData = variables[value];

  if (std::holds_alternative<double>(varData) || std::holds_alternative<bool>(varData) || std::holds_alternative<std::nullptr_t>(varData)) {
    if (lookUp != nullptr) throw std::runtime_error("Runtime error: not an array.");
    else if (arguments.size() != 0) throw std::runtime_error("Runtime error: not a function.");
    else return varData;
  }

  else if (std::holds_alternative<Array>(varData)) {
    if (arguments.size() != 0) throw std::runtime_error("Runtime error: not a function.");

    if (lookUp == nullptr) return varData;
   
    else {
      Array tempArray = std::get<Array>(varData);

      if (!(std::holds_alternative<double>(lookUp->getValue(variables)))) throw std::runtime_error("Runtime error: index is not a number.");

      double arrayIndex = std::get<double>(lookUp->getValue(variables));

      if (std::fmod(arrayIndex, 1) != 0) throw std::runtime_error("Runtime error: index is not an integer.");
      if (arrayIndex >= tempArray->size() || arrayIndex < 0) throw std::runtime_error("Runtime error: index out of bounds.");

      return (*tempArray)[arrayIndex];
    }
  }

  else if (std::holds_alternative<Func>(varData)) {
    if (lookUp != nullptr) throw std::runtime_error("Runtime error: not an array.");

    if (noArgs) return std::get<Func>(varData)->getValue({});

    else if (arguments.size() == 0) return varData;

    else {
      std::vector<Value> tempArgs;

      for (Node* node : arguments) {
        tempArgs.push_back(node->getValue(variables));
      }

      return std::get<Func>(varData)->getValue(tempArgs);
    }
  }

  std::cout << "This error should never happen. In VarNode::getValue" << std::endl;
  exit(2);
}

std::string VarNode::toString() {
  std::ostringstream result;

  result << value;

  if (lookUp != nullptr) result << "[" << lookUp->toString() << "]";
  
  else if (noArgs) result << "()";

  else if (arguments.size() != 0) {
    result << "(";

    for (Node* node : arguments) {
      result << node->toString();

      if (node!= arguments.back()) result << ", ";
    }

    result << ")";
  }

  return result.str();
}

TokenType VarNode::getReturnType([[maybe_unused]] std::map<std::string, Value>& variables) {
  if (std::holds_alternative<bool>(variables[value])) return BOOL;
  else return NUMBER;
}

Value BoolNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  if (lookUp != nullptr) throw std::runtime_error("Runtime error: not an array.");

  return value;
}

std::string BoolNode::toString() {
  std::ostringstream result;

  if (std::get<bool>(value)) result << "true";
  else result << "false";

  if (lookUp != nullptr) result << "[" << lookUp->toString() << "]";

  return result.str();
}

ArrayNode::~ArrayNode() {
  if (value.size() == 0) return;

  for (Node* node : value) {
    delete node;
  }
}

Value ArrayNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  Value result;

  // no array look up
  if (lookUp == nullptr) {
    result = std::make_shared<std::vector<Value>>();

    for (Node* node : value) {
      std::get<Array>(result)->push_back(node->getValue(variables));
    }
  }

  // array lookup
  else {
    if (!(std::holds_alternative<double>(lookUp->getValue(variables)))) throw std::runtime_error("Runtime error: index is not a number.");

    double arrayIndex = std::get<double>(lookUp->getValue(variables));

    if (std::fmod(arrayIndex, 1) != 0) throw std::runtime_error("Runtime error: index is not an integer.");
    if (arrayIndex >= value.size() || arrayIndex < 0) throw std::runtime_error("Runtime error: index out of bounds.");

    result = value[arrayIndex]->getValue(variables);
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

  if (lookUp != nullptr) result << "[" << lookUp->toString() << "]";

  return result.str();
}

Value NullNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {
  if (lookUp != nullptr) throw std::runtime_error("Runtime error: not an array.");

  return value;
}

std::string NullNode::toString() {
  return "null";
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
    if (lhs->lookUp != nullptr) {
      lhs->getValue(variables);
      return rhs->getValue(variables);
    }
    //if (lhs->isValidArrayAssignment) return rhs->getValue(variables);

    std::ostringstream error;
    error << "Runtime error: invalid assignee.";
    throw std::runtime_error(error.str());
  }

  /*
  else if (!(rhs->isVar)) return lhs->getValue(variables);

  else if (rhs->lookUp != nullptr) {
    if (!(std::holds_alternative<double>(rhs->lookUp->getValue(variables)))) throw std::runtime_error("Runtime error: index is not a number.");
    
    double arrayIndex = std::get<double>(rhs->lookUp->getValue(variables));

    if (std::fmod(arrayIndex, 1) != 0) throw std::runtime_error("Runtime error: index is not an integer.");
    if (arrayIndex < 0) throw std::runtime_error("Runtime error: index out of bounds.");
  }
  */

  //std::streambuf* coutBuffer = std::cout.rdbuf();
  //std::stringstream tempStream;
  //std::cout.rdbuf(tempStream.rdbuf());

  try {
    rhs->getValue(variables);
  }
  catch (...) {
    //std::cout.rdbuf(coutBuffer);
    throw;
  }

  //std::cout.rdbuf(coutBuffer);

  return lhs->getValue(variables);
}

TokenType AssignNode::getReturnType([[maybe_unused]] std::map<std::string, Value>& variables) {
  return lhs->getReturnType(variables);
}

Value CompareNode::getValue([[maybe_unused]] std::map<std::string, Value>& variables) {

/*
  if ((value == "==" || value == "!=") && lhs->getReturnType(variables) != rhs->getReturnType(variables)) return false;

  if (std::holds_alternative<double>(lhs->getValue(variables))) {
    if (value == "==") return std::get<double>(lhs->getValue(variables)) == std::get<double>(rhs->getValue(variables));
    else if (value == "!=") return std::get<double>(lhs->getValue(variables)) != std::get<double>(rhs->getValue(variables));
  }

  else if (std::holds_alternative<bool>(lhs->getValue(variables))) {
    if (value == "==") return std::get<bool>(lhs->getValue(variables)) == std::get<bool>(rhs->getValue(variables));
    else if (value == "!=") return std::get<bool>(lhs->getValue(variables)) != std::get<bool>(rhs->getValue(variables));
  }
*/
  if (value == "==") return lhs->getValue(variables) == rhs->getValue(variables);

  if (value == "!=") return lhs->getValue(variables) != rhs->getValue(variables);

/*
  if (lhs->getReturnType(variables) != rhs->getReturnType(variables) || lhs->getReturnType(variables) == BOOL) {
    std::ostringstream error;
    error << "Runtime error: invalid operand type.";
    throw std::runtime_error(error.str());
  }
*/

  if (!(std::holds_alternative<double>(lhs->getValue(variables)) && std::holds_alternative<double>(rhs->getValue(variables)))) {
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

