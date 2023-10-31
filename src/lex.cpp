#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include "lib/lexer.h"

int main() {
    try{
        Lexer lexer = Lexer();
        std::vector<Token> seq= lexer.lexer();
        for(int i = 0; i < (int)(seq.size()); i++){
            std::cout << std::right << std::setw(4) << seq.at(i).line << std::right << std::setw(5) << seq.at(i).column << "  " << seq.at(i).token <<std::endl;
        }
    }
    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    /*std::vector<Token> seq2= lexer.lexer("13 % 7");
    for(int i = 0; i < (int)(seq2.size()); i++){
        std::cout << std::right << std::setw(4) << seq2.at(i).line << std::right << std::setw(5) << seq2.at(i).column << "  " << seq2.at(i).token << std::endl;
    }*/
/*
    std::vector<Token> seq3= lexer.lexer("34 - 45 - 56 - ");
    for(int i = 0; i < (int)(seq3.size()); i++){
        std::cout << std::right << std::setw(4) << seq3.at(i).line << std::right << std::setw(5) << seq3.at(i).column << "  " << seq3.at(i).token << std::endl;
    }*/

   
    return 0;
}