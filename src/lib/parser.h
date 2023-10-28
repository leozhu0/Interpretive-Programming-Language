#include <string>
#include <vector>
#include "token.h"
#include <map>


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
  virtual double getValue();
  std::string toString();
};

struct AssignNode : public OpNode {
  double getValue();
};


struct VarNode : public Node {
 // std::string varValue;
  double getValue();
  std::string toString();
};


class Parser {
  Node* root;

  Node* createNode(std::vector<Token> tokens);
  
public:
  ~Parser();
  void ParserFunc(std::vector<Token> tokens);

  std::string toString();
  double calculate();
};


std::map<std::string, double> variables; //was VarNode*