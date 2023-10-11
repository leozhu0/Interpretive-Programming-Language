#include <string>
#include <vector>
#include "token.h"

struct Node {
  std::string value;
  
  virtual int getValue();
  virtual std::string toString();
};

struct NumNode : public Node {
  int getValue();
  std::string toString();
};

struct OpNode : public Node {
  std::vector<Token> tokens;

  int getValue();
  std::string toString();
};
