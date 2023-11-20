#include <string>
#include <vector>
#include <memory>
#include <map>
#include "token.h"//cpp
#include "node.h"//cpp
#include "value.h"

struct Node {
  Value value;
  TokenType returnType;
  bool isVar = false;

  Node(TokenType type = NUMBER) : returnType(type) {}
  virtual ~Node() {};
  virtual Value getValue([[maybe_unused]] std::map<std::string, Value>& variables) = 0;
  virtual std::string toString() = 0;
  virtual TokenType getReturnType([[maybe_unused]] std::map<std::string, Value>& variables);
};

struct NumNode : public Node {
  Value getValue([[maybe_unused]] std::map<std::string, Value>& variables);
  std::string toString();
};

struct VarNode : public Node {
  std::string value;

  VarNode() {isVar = true;}
  Value getValue([[maybe_unused]] std::map<std::string, Value>& variables);
  std::string toString();
  TokenType getReturnType([[maybe_unused]] std::map<std::string, Value>& variables);
};

struct BoolNode : public Node {
  BoolNode() : Node(BOOL) {}

  Value getValue([[maybe_unused]] std::map<std::string, Value>& variables);
  std::string toString();
};

struct OpNode : public Node {
  std::string value;
  Node* lhs;
  Node* rhs;

  OpNode(TokenType type = NUMBER) : Node(type) {}
  ~OpNode();
  virtual Value getValue([[maybe_unused]] std::map<std::string, Value>& variables);
  std::string toString();
};

struct AssignNode : public OpNode {
  Value getValue([[maybe_unused]] std::map<std::string, Value>& variables);
  TokenType getReturnType([[maybe_unused]] std::map<std::string, Value>& variables);
};

struct CompareNode : public OpNode {
  CompareNode() : OpNode(BOOL) {}

  Value getValue([[maybe_unused]] std::map<std::string, Value>& variables);
};

struct LogicNode : public OpNode {
  LogicNode() : OpNode(BOOL) {}

  Value getValue([[maybe_unused]] std::map<std::string, Value>& variables);
};

//______________________________________________________________________________

class InfixParser {
  Node* root;
  int index = -1;
  size_t parenNum = 0;
  bool updateVariables = true;
  std::vector<std::pair<std::string, Node*>> variableBuffer;
  std::map<std::string, Value>& varCache;

  Node* createTree(Node* leftHandSide, int minPrecedence, std::vector<Token> tokens);
  int precedence(std::string op);
  Token& peak(std::vector<Token> tokens);
  Node* nextNode(std::vector<Token> tokens);
  Value stringToValue(Token& token);

public:
  InfixParser(std::vector<Token> tokens, std::map<std::string, Value>& variables);
  ~InfixParser();

  std::string toString();
  Value calculate();
};
