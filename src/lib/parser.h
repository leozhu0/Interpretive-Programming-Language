#include <string>
#include <vector>

struct Node {
  std::string value;
  
  virtual ~Node();
  virtual double getValue();
  virtual std::string toString();
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

class Parser {
  Node* root;

public:
  ~Parser();
  void createTree(std::vector<Token> tokens);
  Node* createNode(std::vector<Token> tokens);
  std::string toString();
  double calculate();
};

