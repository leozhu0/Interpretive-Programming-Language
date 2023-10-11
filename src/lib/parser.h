#include <string>
#include <vector>

struct Node {
  std::string value;
  
  virtual int getValue();
  virtual std::string toString();
};

struct NumNode : public Node {
  double getValue();
  std::string toString();
};

struct OpNode : public Node {
  std::vector<Node*> children;

  double getValue();
  std::string toString();
};
