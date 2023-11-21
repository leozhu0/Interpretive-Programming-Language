//#pragma once
#include "value.h"
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <sstream>
#include <map>
#include <vector>
#include "run.h"

Value Function::getValue(std::vector<Value> argVals, std::map<std::string, Value> variables){
        //run the code using the arguments
      for(int i = 0; i < (int)argVals.size(); i++){
              variables[arguments[i].token] = argVals[i];
      }
      Scrypt scrypt = Scrypt();
      return scrypt.parseBlock(block, variables);
//        return Value{5.0};//for testing
}


//Value Function::getValue(std::vector<Value> argVals, std::map<std::string, Value> variables){
//	//run the code using the arguments
//	for(int i = 0; i < argVals.size(); i++){
//		variables[arguments[i].token] = argVals[i]; 
//	}
//	Scrypt scrypt = Scrypt();
//	return scrypt.parseBlock(block, variables);
//	return Value{5.0};//for testing
//}


/*std::ostream& operator << (std::ostream& stream, const Value& value) {
    stream << std::visit([](const auto& arg) -> std::string{
        std::stringstream ss;
        ss << arg;
        return ss.str();
    }, value);
    return stream;
}*/
std::ostream& operator << (std::ostream& os, const Value& value) {
  

  std::visit([&os](const auto& tempValue) -> void {
    if constexpr (std::is_same_v<std::decay_t<decltype(tempValue)>, Array>) {
      const std::vector<Value>& tempValues = *tempValue;
      os << "[";

      for (Value value : tempValues) {
        if (value != tempValues.back()) os << value << ", ";
        else os << value;
      }

      os << "]";
    }

    else os << tempValue;

  }, value);

  return os;
}

/*bool operator==(const Value& lhs, const Value& rhs) {
	//std::cout << "TYPE" << rhs.index()<<std::endl;
	//std::cout << "TYPE:" << lhs.index()<<std::endl;
    //if (lhs.index() != rhs.index()) {
        // Different types, return false
      //  return false;
    //
    //
    //
    //}
    
	if(std::holds_alternative<Function>(lhs) || std::holds_alternative<Function>(rhs)){
		return false;
	}
    if (  !((std::holds_alternative<double>(lhs) && std::holds_alternative<double>(rhs)) ||
        (std::holds_alternative<bool>(lhs) && std::holds_alternative<bool>(rhs)) ||
        (std::holds_alternative<Array>(lhs) && std::holds_alternative<Array>(rhs))   ) ) {
		return false;
	}

    // Same types, compare values
    return std::visit([](const auto& a, const auto& b) -> bool {


// 	if constexpr (std::is_same_v<std::decay_t<decltype(a)>, Func> || std::is_same_v<std::decay_t<decltype(b)>, Func>){
  //       return false;
    //     }


	if constexpr (std::is_same_v<std::decay_t<decltype(a)>, Array> && std::is_same_v<std::decay_t<decltype(b)>, Array>) {
	const std::vector<Value>& va = *a;
	const std::vector<Value>& vb = *b;
		if(va.size() != vb.size()){
			return false;
		}
		for(size_t i = 0; i< va.size(); ++i){
			if(va[i] != vb[i]){
				return false;
			}
		}

		return true;

	}


	
        return (a == b);
    }, lhs, rhs);
    
    
}
*/
using ValueBase = std::variant<double,
                                    bool,
                                   std::shared_ptr<Function>,
                                    std::shared_ptr<std::vector<Value>>
                                    >;

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

//std::ostream& operator << (std::ostream& stream, const Value& value);


