#include <vector>
#include <iostream>
#include <string>
#include "lib/infix.h"
#include "lib/lexer.h"

void format(std::vector<Token>& tokens, std::string indent) {
  size_t ifCounter = 0;

  for (size_t i = 0; i < tokens.size(); ++i) {
    if (tokens[i].type == END) break;
    std::cout << indent;

    // expression and print case
    if (tokens[i].type != COMMAND || tokens[i].token == "print") {
      if (tokens[i].token == "print") {
        std::cout << "print ";
	++i;
      }

      int line = tokens[i].line;
      std::vector<Token> tempTokens = {tokens[i]};

      std::cout << "HERE " << tokens.size() << " " << i << std::endl;
      //if (tokens[1].token == "333") exit(0);

      while (true /*i + 1 != tokens.size() && tokens[i + 1].line == line*/) {
        if (i + 1 == tokens.size()) break;
	if (tokens[i + 1].line != line) break;

	tempTokens.push_back(tokens[i + 1]);
        ++i;
      }
      tempTokens.push_back(Token{0, 0, "END", END});

      //std::cout << "__________top" << std::endl;
      //for (Token token : tempTokens) {
      //  std::cout << token.token << " " << token.type << " " << token.line << " " << token.column << std::endl;
      //}
      //std::cout << "__________bottom" << std::endl;
      std::cout << "THERE " << std::endl;
      if (tempTokens.back().token == "333") exit(0);

      InfixParser parser = InfixParser(tempTokens);
      std::cout << parser.toString() << std::endl;
    }

    // while and if case
    else if (tokens[i].token != "else" && tokens[i].token != "else if") {
      if (tokens[i].token == "if") {
        if (ifCounter != 0) {
	  //TODO
	}
	
        std::cout << "if ";
        ++ifCounter;
      }	

      else std::cout << "while ";

      ++i;
      std::vector<Token> condition;

      while (tokens[i].token != "{") {
        condition.push_back(tokens[i]);
	++i;
      }
      condition.push_back(Token{0, 0, "END", END});

      /*
      for (Token token : condition) {
        std::cout << token.token << " " << token.type << std::endl;
      }
      */

      ++i;
      InfixParser parser = InfixParser(condition);
      std::cout << parser.toString() << " {" << std::endl;

      size_t numCurly = 1;
      std::vector<Token> body;

      while (true) {
	if (tokens[i].token == "{") ++numCurly;
	else if (tokens[i].token == "}") --numCurly;

        if (numCurly == 0) break;
        body.push_back(tokens[i]);
	++i;
      }

      format(body, indent + "    ");
      std::cout << indent << "}" << std::endl;
    }

    // else case
    else if (tokens[i].token == "else" || tokens[i].token == "else if") {
      if (ifCounter == 0) {
        //TODO
      }

      --ifCounter;
      std::cout << "else {" << std::endl;

      bool isElseIf = (tokens[i].token == "else if" ? true : false);
      size_t numCurly = 1;
      std::vector<Token> body;
      //bool endingCurly = false;

      if (isElseIf) {
        body.push_back(Token{0, 0, "if", COMMAND});
	//endingCurly = true;
	--numCurly;
      }
      else ++i;

      ++i;

      while (true) {
        if (tokens[i].token == "{") ++numCurly;
        else if (tokens[i].token == "}") --numCurly;

        if (numCurly == 0) {
	  if (!isElseIf || (tokens[i + 1].token != "else" && tokens[i + 1].token != "else if")) break;

	  while (tokens[i + 1].token != "{") {
            body.push_back(tokens[i]);
	    ++i;
	  }
	}

        body.push_back(tokens[i]);
        ++i;
      }

      if (isElseIf) body.push_back(tokens[i]);

      std::cout << "__________top else" << std::endl;
      for (Token token : body) {
	std::cout << token.token << " " << token.type << std::endl;
      }
      std::cout << "__________bottom else" << std::endl;

      format(body, indent + "    ");
      std::cout << indent << "}" << std::endl;
    }

    else {
      std::cout << "this formatting error should never happen" << std::endl;
      exit(1);
    }
  }
}

int main() {
    Lexer lexer = Lexer();

    std::vector<Token> tokens = lexer.lexer();
    
    //std::cout << "__________top" << std::endl;
    //for (Token token : tokens) {
    //  std::cout << token.token << " " << token.type << " " << token.line << " " << token.column << std::endl;
    //}
    //std::cout << "__________bottom" << std::endl;

    format(tokens, "");

    return 0;
}
