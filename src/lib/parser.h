#include <string>
#include <vector>
#include "token.h"

struct Node {
  std::string value;
  
  virtual ~Node() {};
  virtual double getValue() = 0;
  virtual std::string toString() = 0;
};

struct NumNode : public Node {
  double getValue();
  std::string toString();
};

struct OpNode : public Node {
  std::vector<Node*> children;

  ~OpNode();
  double getValue();
  std::string toString();
};

struct AssignNode : public Node {
  // TODO
  ;
}

class Parser {
  Node* root;

public:
  ~Parser();
  Parser(std::vector<Token> tokens);

  Node* createNode(std::vector<Token> tokens);
  std::string toString();
  double calculate();
};

