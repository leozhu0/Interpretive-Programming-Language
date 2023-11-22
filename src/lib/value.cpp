#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <sstream>
#include <map>
#include <vector>
#include "run.h"

Value Function::getValue(std::vector<Value> argVals, std::map<std::string, Value> variables){

  if((int)block.size() == 0){
	  return nullptr;
  }
  //Combine the var names and value arguments to make variables
  for(int i = 0; i < (int)argVals.size(); i++){
     //Add these variables to the map
     variables[arguments[i].token] = argVals[i];
  }

  Scrypt scrypt = Scrypt();
  return scrypt.parseBlock(block, variables);
}


std::ostream& operator << (std::ostream& os, const Value& value) {
   //Don't compare functions
   if (std::holds_alternative<Func>(value)) {
    return os;
  }

  if (std::holds_alternative<std::nullptr_t>(value)) {
    os << "null";
    return os;
  }

  else if (std::holds_alternative<bool>(value)) {
    if (std::get<bool>(value)) os << "true";
    else os << "false";

    return os;
  }

  std::visit([&os](const auto& tempValue) -> void {
    if constexpr (std::is_same_v<std::decay_t<decltype(tempValue)>, Array>) {
      const std::vector<Value>& tempValues = *tempValue;
      os << "[";

	for (size_t i = 0; i < tempValues.size(); ++i) {
  		if (i != tempValues.size() - 1) os << tempValues.at(i) << ", ";
  		else os << tempValues.at(i);
	}
      os << "]";
    }

    else os << tempValue;

  }, value);

  return os;
}

//Base type for value (Used  in the == operator)
using ValueBase = std::variant<double,
                               bool,
                               std::shared_ptr<Function>,
                               std::shared_ptr<std::vector<Value>>,
			       std::nullptr_t>;

bool operator == (const Value& lhs, const Value& rhs){
	const Array *a = std::get_if<Array>(&lhs);
	const Array *b = std::get_if<Array>(&rhs);

	if(a != nullptr && b != nullptr){
		return (**a == **b);
	}

	return ((const ValueBase) lhs == (const ValueBase) rhs);
}

bool operator!=(const Value& lhs, const Value& rhs) {
	return !(lhs == rhs);

 }



