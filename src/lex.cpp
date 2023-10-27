#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "lib/lexer.cpp"

int main() {
    Lexer lexer = Lexer();
    
    std::vector<Token> seq= lexer.lexer();
    for(int i = 0; i < (int)(seq.size()); i++){
        std::cout << std::right << std::setw(4) << seq.at(i).line << std::right << std::setw(5) << seq.at(i).column << "  " << seq.at(i).token << std::endl;
    }
    return 0;
}