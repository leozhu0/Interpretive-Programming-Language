struct Node {
  string value;
  
  virtual int getValue();
  virtual string toString();
};

struct NumNode : public Node {
  int getValue();
  int toString();
};

struct OpNode : public Node {
  int getValue();
  int toString();
};
