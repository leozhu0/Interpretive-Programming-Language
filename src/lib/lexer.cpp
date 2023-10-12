#include <string>
#include <iostream>
#include <vector>
#include <fstream>
//#include "parser.h"
#include "token.h"


class Lexer {

    private:
        std::ifstream file;

         TokenType tokentype(char token){
            
            if(token == '.' || ((int)token >= 48 && (int)token <= 57) ){
                return NUMBER;
            }

            if(token == ')' || token == '(') {
                return PARENTHESIS;
            }

             if(token == '+' || token == '-' || token == '*' || token == '/') {
                return OPERATOR;
            }

            return NULLTYPE;
        }

        void pushseq(std::string element, TokenType type, int line, int &column, std::vector<Token> &sequence){
            if(element != ""){
                sequence.push_back(Token{line+1,column, element, type});
                column++;
            }
        }

       

    public:
        Lexer(std::string filename){
            file.open(filename);

          /*   if (!inputFile.is_open()) {
                std::cerr << "Error opening the file: " << filename << std::endl;
                return 1;
            }*/
        }

        std::vector<Token> lexer(){
            //std::string raw_input

            std::vector<Token> sequence;
            
            //bool decimalExists = false;
            std::string raw_input;
            int line = 0;
            while (std::getline(file, raw_input)) {
                int column = 1;
                std::string element = "";
                for(int i = 0; i < raw_input.length(); i++){
                    TokenType type = tokentype(raw_input[i]);
                    if(type != NULLTYPE){
                        if(raw_input[i] == '.' || ((int)raw_input[i] >= 48 && (int)raw_input[i] <= 57)) {
                            element += raw_input[i];
                            /*if(raw_input[i] == '.' && decimalExists == true){
                                return NULL; //, "Invalid float, more than one decimals";
                            }

                            if(raw_input[i] == '.'){
                                decimalExists = true;
                            }*/
                            
                        } else {
                            /*if(element != ""){
                                sequence.push_back(Token{line+1,column,element, NUMBER});
                                column++;
                            }*/
                            pushseq(element, NUMBER, line, column, sequence);
                            pushseq(raw_input.substr(i, 1), type, line, column, sequence);
                            /*if(raw_input.substr(i, 1) != ""){
                                sequence.push_back(Token{line+1,column,raw_input.substr(i, 1), type});
                                column++;
                            }*/
                            
                            element = "";
                        }
                    } else {
                        /*if(element != ""){
                            sequence.push_back(Token{line+1,column,element, NUMBER});
                            column++;
                        }*/pushseq(element, NUMBER, line, column, sequence);
                        

                            element = "";
                    }

                    if(i == raw_input.length()-1 && tokentype(element[0]) != NULLTYPE){ //ALWAYS NUMBER 
                        sequence.push_back(Token{line+1,column,element, tokentype(element[0])});
                        column++;
                    }

                }
                element = "";
                line++;
            }

            file.close();

            sequence.push_back(Token{line+2,1,"END", END});


            return sequence;
        }


};




//Syntax error on line 1 column 9.
/*int main() {
    Lexer lexer = Lexer("code.txt");
    
    std::vector<Token> seq= lexer.lexer();
    for(int i = 0; i < seq.size(); i++){
        std::cout << seq.at(i).line << " " << seq.at(i).column << " " << seq.at(i).token << " " <<  seq.at(i).type << std::endl;
    }
    return 0;
}*/