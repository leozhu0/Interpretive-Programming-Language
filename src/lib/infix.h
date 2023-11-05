#include <string>
#include <vector>
#include <memory>
#include "token.h"
#include "node.h"

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

public:
  InfixParser(std::vector<Token> tokens);
  ~InfixParser();

  std::string toString();
  double calculate();
};
