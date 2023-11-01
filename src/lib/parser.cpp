#include "parser.h"
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>

std::map<std::string, double> variables;

Parser::~Parser() {
  delete root;
}

void Parser::ParserFunc(std::vector<Token> tokens) {
  if (tokens.size() == 0) {
    std::cout << "No tokens" << std::endl;
    exit(2);
  }

  root = createNode(tokens);
}

bool stringOfDouble(std::string attempt){
    try {
        double number = std::stod(attempt);
        return number;
    } catch (const std::invalid_argument& e) {
        return 0;
    } catch (const std::out_of_range& e) {
        return 0;
    }
    return 0;
}

Node* Parser::createNode(std::vector<Token> tokens) {
  size_t start = 0;



  // If the expression is just a single number
  if (tokens[start].type == NUMBER) {
    if (tokens.size() > 2) {
      std::cout << "1Unexpected token at line " << tokens[1].line << " column " << tokens[1].column << ": " << tokens[1].token << std::endl;
      exit(2);
    }

    NumNode* node = new NumNode;
    node->value = tokens[start].token;

    return node;

  // If the expression has an operator
  } else if (tokens[start].token != "("){
      std::cout << "2Unexpected token at line " << tokens[start].line << " column " << tokens[start].column << ": " << tokens[start].token << std::endl;
      exit(2);
  }else if (tokens[start].token == "(") {
      if (tokens[start + 1].type != OPERATOR && tokens[start + 1].type != ASSIGNMENT) {
        std::cout << "3Unexpected token at line " << tokens[start + 1].line << " column " << tokens[start + 1].column << ": " << tokens[start + 1].token << std::endl;
        exit(2);
      }


  if(tokens[start + 1].type == OPERATOR){
    ++start;
    // variable to check for parenthesis error
    int allowedParenthesis = 1;
    OpNode* node = new OpNode;
    
    
    node->value = tokens[start].token;

    // iterates through everything following the operator
    for (size_t i = start + 1; i < tokens.size(); ++i) {
      if (allowedParenthesis == 0 && i != tokens.size() - 1) {
        std::cout << "4Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
        exit(2);
      }

      if (tokens[i].token == ")") {
        if (i == start + 1) {
          std::cout << "5Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
          exit(2);
        }
        //std::cout << "MUNU";
        --allowedParenthesis;
      }
      
      // adding a number to the child pointers
      else if (tokens[i].type == NUMBER) {
        NumNode* tempNode = new NumNode;
        tempNode->value = tokens[i].token;
        node->children.push_back(tempNode);
      } else if (tokens[i].type == VARIABLE) {
        VarNode* tempNode = new VarNode;
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
              std::cout << "6Unexpected token at line " << tokens[i - 1].line << " column " << tokens[i - 1].column << ": " << tokens[i - 1].token << std::endl;
              exit(2);
            }
          }

          tempTokens.push_back(tokens[tokens.size() - 1]);
          node->children.push_back(createNode(tempTokens));
      }

      else if (tokens[i].type == OPERATOR || tokens[i].type == ASSIGNMENT) {
        std::cout << "7Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
        exit(2);
      }

    }
    //std::cout << "ALLOW: " << allowedParenthesis << std::endl;

    if (!(allowedParenthesis <= 0)) {
      std::cout << "8Unexpected token at line " << tokens[tokens.size() - 2].line << " column " << tokens[tokens.size() - 2].column + tokens[tokens.size() - 2].token.size() << ": " << tokens[tokens.size() - 1].token << std::endl;
      exit(2);
    }

    return node;
    } 
    
    
    else if (tokens[start + 1].type == ASSIGNMENT){
      
          //////////////////////////////////////
            //LOOK INTO THIS LATER
            // int tempErrorParens = 1;
            // int lastIndexOfSpan;
            // //last open index 
            // int lastOpenIndex = 0;
            
            // for(lastIndexOfSpan = 1; tempErrorParens != 0; lastIndexOfSpan++){
            //   if(tokens[start + 1 + lastIndexOfSpan].token == ")"){
            //     tempErrorParens--;
            //   }
            //   if(tokens[start + 1 + lastIndexOfSpan].token == "("){
            //     tempErrorParens++;
            //     lastOpenIndex = lastIndexOfSpan;
            //   }

            // }

            // if(lastOpenIndex == 0){
            //   lastOpenIndex = lastIndexOfSpan-2;
            // }
        //std::cout << lastIndexOfSpan;

    //  std::vector<Token> tempSpan;
    //  for(int s = 0; s < lastIndexOfSpan; s++){
    //   if(tokens[start + 1 + s].token == "("){
    //     std::vector<Token> subSpan;
    //     for(int sub = s; tokens[start + 1 + s - 1].token != ")"; s++){
    //         subSpan.push_back(tokens[start + 1 + s]);
    //     }
    //     tempSpan.push_back(EVAL(subSpan));
    //   }
    //  }
    

      

      /*if(tokens[start + 1 + lastIndexOfSpan - 1].type == VARIABLE){
        if(variables.find(tokens[start + 1 + lastIndexOfSpan - 1].token) == variables.end()){
            std::ostringstream error;
            error <<"2Unexpected token at line "<<tokens[start + 1 +lastIndexOfSpan].line<<" column "<<tokens[start + 1 + lastIndexOfSpan].column<<": " <<tokens[start + 1 + lastIndexOfSpan].token<<std::endl;
            throw std::runtime_error(error.str());
        }
      }*/
//ignore
            //std::cout << lastOpenIndex << std::endl;
          
            //tempErrorParens = 1;
            //bool oneNum = 0;
    //ignore      
          
          
          
            // for(int t = 1; t < lastOpenIndex; t++){
            //   if(tokens[start + 1 + t].type != VARIABLE){
            //     std::ostringstream error;
            //     error <<"1Unexpected token at line "<<tokens[start + 1 + t].line<<" column "<<tokens[start + 1 + t].column<<": " <<tokens[start + 1 + t].token<<std::endl;
            //     throw std::runtime_error(error.str());
            //   }
              
            // }

            /*
            for(int t = 1; tempErrorParens != 0; t++){
              if((tokens[start + 1 + t].token == "(" || tokens[start + 1 + t].type == NUMBER) && oneNum == 1){
                      std::cout <<"Unexpected token at line "<<tokens[start + 1 + t].line<<" column "<<tokens[start + 1 + t].column<<": " <<tokens[start + 1 + t].token<<std::endl;
                      exit(2);
              }

              if(tokens[start + 1 + t].token == ")"){
                tempErrorParens--;
              }
              if(tokens[start + 1 + t].token == "("){
                tempErrorParens++;
                oneNum = 1;
              }
              if(tokens[start + 1 + t].type == NUMBER){
                oneNum = 1;
              }
              
            }
            */




///////////////////////////////////////////////////////////

        ++start;
        // variable to check for parenthesis error
        int allowedParenthesis = 1;
        OpNode* node = new AssignNode;
        
        node->value = tokens[start].token;

        bool allowNum = 1;
        // iterates through everything following the operator
        for (size_t i = start + 1; i < tokens.size(); ++i) {
          if(!allowNum ){
            if(tokens[i].token == ")" && i == tokens.size()-1){
              allowedParenthesis--;
              break;
            }else{
              std::cout << "9Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
              exit(2);
            }
          }

          if (allowedParenthesis == 0 && i != tokens.size() - 1) {
            std::cout << "10Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
            exit(2);
          }

          if (tokens[i].token == ")") {
            if (i == start + 1) {
              std::cout << "11Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
              exit(2);
            }

            --allowedParenthesis;
          }
          
          // adding a number to the child pointers
          else if (tokens[i].type == NUMBER) {
            NumNode* tempNode = new NumNode;
            tempNode->value = tokens[i].token;
            node->children.push_back(tempNode);
            allowNum = 0;
          }
          else if (tokens[i].type == VARIABLE) {
            VarNode* tempNode = new VarNode;
            tempNode->value = tokens[i].token;
            node->children.push_back(tempNode);
          }
        

        

          // adding an operator to the child pointers
          else if (tokens[i].token == "(") {
            allowNum =0;
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
                  std::cout << "12Unexpected token at line " << tokens[i - 1].line << " column " << tokens[i - 1].column << ": " << tokens[i - 1].token << std::endl;
                  exit(2);
                }
              }

              

              tempTokens.push_back(tokens[tokens.size() - 1]);
              
              

              node->children.push_back(createNode(tempTokens));
          }

          else if (tokens[i].type == OPERATOR || tokens[i].type == ASSIGNMENT) {
            std::cout << "13Unexpected token at line " << tokens[i].line << " column " << tokens[i].column << ": " << tokens[i].token << std::endl;
            exit(2);
          }

        }


      for(int t = 0; t < (int)(node->children.size() - 1); t++){
        
          variables[node->children.at(t)->value] = node->children.at(node->children.size() - 1)->getValue();
        }

            

      if (!(allowedParenthesis == 0)) {
        std::cout << "14Unexpected token at line " << tokens[tokens.size() - 2].line << " column " << tokens[tokens.size() - 2].column + tokens[tokens.size() - 2].token.size() << ": " << tokens[tokens.size() - 1].token << std::endl;
        exit(2);
      }

      return node;

    // default error case
    }  else {
      std::cout << "15Unexpected token at line " << tokens[start].line << " column " << tokens[start].column << ": " << tokens[start].token << std::endl;
      exit(2);
    }

          
  } else {
    std::cout << "16Unexpected token at line " << tokens[start + 1].line << " column " << tokens[start + 1].column << ": " << tokens[start + 1].token << std::endl;
    exit(2);
  }
  
    
  exit(1);
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
        std::ostringstream error;
        error << "Runtime error: division by zero.";
        throw std::runtime_error(error.str());
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


double VarNode::getValue(){
  if(variables.find(value) == variables.end()){
    std::ostringstream error;
    error <<"Runtime error: unknown identifier " << value;
    throw std::runtime_error(error.str());
  }

   return variables[value];
}

std::string VarNode::toString(){
   return value;
}


double AssignNode::getValue() {
    return children.at(0)->getValue();
}
