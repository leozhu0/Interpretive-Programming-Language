#include <vector>
#include <iostream>
#include <fstream>
#include "token.h"

class Lexer {
    private:
       // std::string input;
        TokenType tokentype(char token);
        void pushseq(std::string element, TokenType type, int line, int column, std::vector<Token> &sequence);

    public:
        //Lexer();
        std::vector<Token> lexer();
};




