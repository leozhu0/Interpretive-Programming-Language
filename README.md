# What the project does, and why.
Our project inputs mathematics equations through the terminal, runs it through a lexer, condtructs an AST, parses it, and outputs the result to standard output.
We built this software in a way such that anyone can use it to evaulate math expressions quickly. 

# How to build the project (the fewer commands the better; a single make command is ideal).
To compile the project into an executable, just cd into the project directory and run:

g++ parser.cpp -o parser


# How to use the executables once they're built.
Stay in the project directory and run

./parser 

This will begin the multiline input.
Provide your input, hit (^Z) to terminate input.
You will then see the output.

# An overview of how the code is organized.
All the code is stored inside the src/ folder.

Within src, the files containing the main functions are the files outside of /lib (lex.cpp, parse.cpp, calc.cpp)

src/lib. contains the source code, each class has its own header file and .cpp file for implemetation.

infix.h and infix.cpp holds the OOP implemetation for the infix parser

lexer.h and lexer.cpp holds the OOP implemetation for the lexer

parser.h and lexer.cpp holds the OOP implemetation for the parser

token.h and lexer.cpp holds the OOP implemetation for the lexer

All the files use token.h; infix and parser use lexer.h; infix uses parser.h