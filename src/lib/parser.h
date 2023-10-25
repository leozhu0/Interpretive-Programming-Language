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

  Node* createNode(std::vector<Token> tokens);

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
