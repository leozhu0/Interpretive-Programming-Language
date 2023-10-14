#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "lib/lexer.cpp"

int main() {
    Lexer lexer = Lexer("code.txt");
    
    std::vector<Token> seq= lexer.lexer();
    for(int i = 0; i < (int)(seq.size()); i++){
        std::cout << seq.at(i).line << " " << seq.at(i).column << " " << seq.at(i).token << std::endl;
    }
    return 0;
}