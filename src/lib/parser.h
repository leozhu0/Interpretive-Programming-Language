#include <string>
#include <vector>
#include "token.h"
#include <map>

enum NodeType {
  NUMNODE,
  OPNODE,
  ASSIGNMENTNODE,
  UNKNOWNNODE
};
struct Node {
  std::string value;
  NodeType type = UNKNOWNNODE;
  virtual ~Node() {};
  virtual double getValue() = 0;
  virtual std::string toString() = 0;
};

struct NumNode : public Node {
   NodeType type = NUMNODE;
  double getValue();
  std::string toString();
};

struct OpNode : public Node {
   NodeType type = OPNODE;
  std::vector<Node*> children;

  ~OpNode();
  virtual double getValue();
  std::string toString();
};

struct AssignNode : public OpNode {
   NodeType type = ASSIGNMENTNODE;
  double getValue();
};

class Parser {
  Node* root;

  Node* createNode(std::vector<Token> tokens);

  std::map<AssignNode*, double> variables;


public:
  ~Parser();
  Parser(std::vector<Token> tokens);

  std::string toString();
  double calculate();
};

class InfixParser {
  Node* root;
  static size_t index = 0;

  Node* createTree(Node* leftHandSide, int minPrecedence, std::vector<Token> tokens);
  int precedence(std::string op);
  Token& peak(std::vector<Token> tokens);
  Node* nextNode(std::vector<Token> tokens);

public:
  InfixParser(std::vector<Token> tokens);
  ~InfixParser();

  std::string toString();
  double calculate();
};
