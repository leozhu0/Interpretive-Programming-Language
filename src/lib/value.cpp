#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <sstream>
#include <map>
#include <vector>
#include "run.h"
#include <stdexcept>

Value Function::getValue(std::vector<Value> argVals){

  if((int)arguments.size() != (int)argVals.size()){
	std::ostringstream error;
	error << "Runtime error: incorrect argument count.";
	throw std::runtime_error(error.str());
  }

  std::map<std::string, Value> variablesCopy = variables;
  if((int)block.size() == 0){
	  return nullptr;
  }
  //Combine the var names and value arguments to make variables
  for(int i = 0; i < (int)argVals.size(); i++){
     //Add these variables to the map
     variablesCopy[arguments[i].token] = argVals[i];
  }
  Scrypt scrypt = Scrypt();

  Value output;	
  try{
      scrypt.parseBlock(block, variablesCopy, true); 
  }
  catch (Value value){
	output = value;
  }


  return output;
}


Function::Function(std::vector<Token> arguments_a, std::vector<Token> block_a, std::map<std::string, Value> variables_a){
     arguments = arguments_a;
     block = block_a;
     variables = variables_a;
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



