#include "parser.h"
#include "token.h"

int Node::getValue() {
  return 0;
}

string Node::toString() {
  return "If you see this, you messed up";
}

int NumNode::getValue() {
  return value; 
}
    
string NumNode::toString() {
  return 

