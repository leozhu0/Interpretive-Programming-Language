#include "lib/run.h"
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
        scrypt.parseBlock(tokens, variables, false);
    }
    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
      return 3;
    } catch(...){}

    return 0;
}
