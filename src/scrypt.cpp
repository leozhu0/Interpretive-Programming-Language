#include "lib/run.h"
//#include "lib/lexer.h" // cpp
//#include "lib/infix.cpp" //cpp
//#include "lib/value.cpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>

int main() {
    std::vector<Token> tokens;
    try {
        Lexer lexer = Lexer();
        tokens = lexer.lexer();
    }
    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      exit(1);
    }

    try {
	Scrypt scrypt = Scrypt();
        std::map<std::string, Value> variables;
        scrypt.parseBlock(tokens, variables);
    }
    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      return 2;
    }

    return 0;
}
