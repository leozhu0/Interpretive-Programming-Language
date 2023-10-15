#include <vector>
#include <fstream>

class Lexer {
    private:
        std::ifstream file;

        TokenType tokentype(char token);

        void pushseq(std::string element, TokenType type, int line, int column, std::vector<Token> &sequence);

    public:
        Lexer(std::string filename);

        std::vector<Token> lexer();

};




