#include <vector>
#include <iostream>
#include <string>
#include "lib/infix.h"
#include "lib/lexer.h"
#include "lib/value.h"

void format(std::vector<Token>& tokens, std::string indent) {
  if (tokens.back().type == END) {
    if (tokens[tokens.size() - 2].token != "}" || tokens[tokens.size() - 2].token != ";") {
      std::ostringstream error;
      error << "Unexpected token at line " << tokens[tokens.size() - 2].line << " column " << tokens[tokens.size() - 2].column << ": " << tokens[tokens.size() - 2].token;
      throw std::runtime_error(error.str());
    }
  }

  else if (tokens.back().token != "}" || tokens.back().token != ";") {
    std::ostringstream error;
    error << "Unexpected token at line " << tokens.back().line << " column " << tokens.back().column << ": " << tokens.back().token;
    throw std::runtime_error(error.str());
  }

  size_t ifCounter = 0;
  std::map<std::string, Value> variables;

  // loops through the entire vector of tokens
  for (size_t i = 0; i < tokens.size(); ++i) {
    // end case
    if (tokens[i].type == END) break;
    std::cout << indent;

    // expression, print, and return case
    if ((tokens[i].type != COMMAND && tokens[i].type != FUNCTION) || tokens[i].token == "print") {
      if (tokens[i].token == "print") {
        std::cout << "print";

	if (tokens[i + 1].token == ";") {
          std::cout << ";" << std::endl;
	  continue;
	}

	else std::cout << " ";

	++i;
      }

      else if (tokens[i].token == "return") {
        std::cout << "return";

        if (tokens[i + 1].token == ";") {
          std::cout << ";" << std::endl;
	  ++i;
          continue;
        }

        else std::cout << " ";

	++i;
      }

      std::vector<Token> tempTokens{tokens[i]};
      ++i;

      // potentially need to add the case where the vector ends with a ";"
      while (tokens[i].token != ";") {
	tempTokens.push_back(tokens[i]);
	++i;
      }

      tempTokens.push_back(tokens[i]);
      tempTokens.back().token = "END";
      tempTokens.back().type = END;

      InfixParser parser = InfixParser(tempTokens, variables);
      std::cout << parser.toString() << ";" << std::endl;
    }

    // while, if, and def case
    else if (tokens[i].token != "else" && tokens[i].token != "else if") {
      if (tokens[i].token == "if") {
        if (ifCounter != 0) {
	  // Not checked
	}
	
        std::cout << "if ";
        ++ifCounter;
      }	

      else if (tokens[i].token == "while") std::cout << "while ";

      else std::cout << "def ";

      ++i;
      std::vector<Token> condition;

      while (tokens[i].token != "{") {
        condition.push_back(tokens[i]);
	++i;
      }
      condition.push_back(Token{0, 0, "END", END});

      ++i;
      InfixParser parser = InfixParser(condition, variables);
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
        // Not checked
      }

      --ifCounter;
      std::cout << "else {" << std::endl;

      bool isElseIf = (tokens[i].token == "else if" ? true : false);
      size_t numCurly = 1;
      std::vector<Token> body;

      ++i;

      // else if case needs to include if condition
      // this loop does this
      if (isElseIf) {
        body.push_back(Token{0, 0, "if", COMMAND});

	while (tokens[i].token != "{") {
	  body.push_back(tokens[i]);
	  ++i;
	}

	body.push_back(tokens[i]);
      }

      ++i;

      while (true) {
        if (tokens[i].token == "{") ++numCurly;
        else if (tokens[i].token == "}") --numCurly;

        if (numCurly == 0) {
	  if (!isElseIf || i == tokens.size() - 1 || (tokens[i + 1].token != "else" && tokens[i + 1].token != "else if")) break;

	  // else if case needs to consider whether or not the following command is else or else if
	  // if it is, it needs to be included before called recursively
	  while (tokens[i + 1].token != "{") {
            body.push_back(tokens[i]);
	    ++i;
	  }
	}

        body.push_back(tokens[i]);
        ++i;
      }

      if (isElseIf) body.push_back(tokens[i]);

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
    format(tokens, "");
  }
  catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return 2;
  }

  return 0;
}
