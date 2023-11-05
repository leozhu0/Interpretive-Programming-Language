

int main() {
    Lexer lexer = Lexer();

    std::vector<Token> tokens = lexer.lexer();


    for(int i = 0; i < (int)(tokens.size()); i++){
            std::cout << std::right << std::setw(4) << tokens.at(i).line << std::right << std::setw(5) << tokens.at(i).column << "  " << tokens.at(i).token <<std::endl;
    }

    int i = 0;
    ParseBlock(tokens, i);

    return 0;
}
