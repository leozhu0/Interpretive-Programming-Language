/*
#pragma once

#include <string>
#include <vector>
#include "token.h"
#include <map>

struct Node {
  std::string value;
  TokenType returnType;

  Node(TokenType type = NUMBER) : returnType(type) {}
  virtual ~Node() {};
  virtual double getValue() = 0;
  virtual std::string toString() = 0;
};

struct NumNode : public Node {
  double getValue();
  std::string toString();
};
 
struct VarNode : public Node {
  double getValue();
  std::string toString();
};

struct BoolNode : public Node {
  BoolNode() : Node(BOOL) {}

  double getValue();
  std::string toString();
};

struct OpNode : public Node {
  Node* lhs;
  Node* rhs;

  OpNode(TokenType type = NUMBER) : Node(type) {}
  ~OpNode();
  virtual double getValue();
  std::string toString();
};

struct AssignNode : public OpNode {
  double getValue();
};

struct CompareNode : public OpNode {
  CompareNode() : OpNode(BOOL) {}

  double getValue();
};

struct LogicNode : public OpNode {
  LogicNode() : OpNode(BOOL) {}

  double getValue();
};

//extern std::map<std::string, double> variables;
*/
