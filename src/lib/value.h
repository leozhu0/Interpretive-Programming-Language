#ifndef VALUE_H
#define VALUE_H
#include <variant>
#include <memory>
#include <ostream>
#include <vector>
#include "token.h"
#include <sstream>
#include <map>

struct Value;

class Function {
    public:
        std::vector<Token> arguments;
        std::vector<Token> block;
        std::map<std::string, Value> variables;
        Value getValue(std::vector<Value> argVals);
};

//class Function;
struct Value : public std::variant<
  double, 
  bool,
  std::shared_ptr<Function>,
  std::shared_ptr<std::vector<Value>>,//,
  std::nullptr_t
> {
  using variant::variant;
};

using Array = std::shared_ptr<std::vector<Value>>;
using Func = std::shared_ptr<Function>;

std::ostream& operator << (std::ostream& stream, const Value& value);

bool operator==(const Value& lhs, const Value& rhs);
bool operator!=(const Value& lhs, const Value& rhs);

#endif
