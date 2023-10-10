#include <string>

struct Node {
  std::string value;
  
  virtual int getValue();
  virtual std::string toString();
};

struct NumNode : public Node {
  int getValue();
  std::string toString();
};

struct OpNode : public Node {
  int getValue();
  std::string toString();
};
