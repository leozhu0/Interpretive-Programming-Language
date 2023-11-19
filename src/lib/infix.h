#include <string>
#include <vector>
#include <memory>
#include "token.h"//cpp
#include "node.h"//cpp
#include "value.h"

struct Node {
  Value value;
  TokenType returnType;
  bool isVar = false;

  Node(TokenType type = NUMBER) : returnType(type) {}
  virtual ~Node() {};
  virtual Value getValue() = 0;
  virtual std::string toString() = 0;
  virtual TokenType getReturnType();
};

struct NumNode : public Node {
  Value getValue();
  std::string toString();
};

struct VarNode : public Node {
  std::string value;

  VarNode() {isVar = true;}
  Value getValue();
  std::string toString();
  TokenType getReturnType();
};

struct BoolNode : public Node {
  BoolNode() : Node(BOOL) {}

  Value getValue();
  std::string toString();
};

struct OpNode : public Node {
  std::string value;
  Node* lhs;
  Node* rhs;

  OpNode(TokenType type = NUMBER) : Node(type) {}
  ~OpNode();
  virtual Value getValue();
  std::string toString();
};

struct AssignNode : public OpNode {
  Value getValue();
  TokenType getReturnType();
};

struct CompareNode : public OpNode {
  CompareNode() : OpNode(BOOL) {}

  Value getValue();
};

struct LogicNode : public OpNode {
  LogicNode() : OpNode(BOOL) {}

  Value getValue();
};

//______________________________________________________________________________

class InfixParser {
  Node* root;
  int index = -1;
  size_t parenNum = 0;
  bool updateVariables = true;
  std::vector<std::pair<std::string, Node*>> variableBuffer;

  Node* createTree(Node* leftHandSide, int minPrecedence, std::vector<Token> tokens);
  int precedence(std::string op);
  Token& peak(std::vector<Token> tokens);
  Node* nextNode(std::vector<Token> tokens);
  Value stringtoValue(Token token);

public:
  InfixParser(std::vector<Token> tokens);
  ~InfixParser();

  std::string toString();
  Value calculate();
};
