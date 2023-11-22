# What the project does, and why.
Our project inputs basic code through the terminal, runs it through a lexer, condtructs an AST, parses it, and outputs the result to standard output.
We built this software in a way such that anyone can use it to run an interpreted programming language that contain math expressions.

# How to build the project (the fewer commands the better; a single make command is ideal).
To compile the project into an executable, just cd into the project directory and run:
```
g++ lib/*.cpp src/scrypt.cpp -o scrypt
```

# How to use the executables once they're built.
Stay in the project directory and run

./scrypt

This will begin the multiline input.
Provide your input, hit (^Z) to terminate input.
You will then see the output.

# An overview of how the code is organized.
All the code is stored inside the src/ folder.

Within src, the files containing the main functions are the files outside of /lib (lex.cpp, calc.cpp, scrypt.cpp, format.cpp)

src/lib. contains the source code, each class has its own header file and .cpp file for implemetation.

infix.h and infix.cpp holds the OOP implemetation for the infix parser

lexer.h and lexer.cpp holds the OOP implemetation for the lexer

token.h and lexer.cpp holds the OOP implemetation for the lexer

run.h and run.cpp holds the OOP implemetation for the parser

value.h and value.cpp holds the OOP implemtaton for the Value type, this is a universal type of functions, arrays, doubles, bools, and nullptrs

node.h and node.cpp implements the node classes which are used throughout the rest of the program.

All the files use token.h; infix and parser use lexer.h; infix uses parser.h

Scrypt is for the main executable while format just prints outs a restructured verson of the input.

# Example
```
a = [1, 2, 3];
def func(a, b, arr){
    return arr[a + b];
}

if 1 >= 0 {
    i = 10;
    while i > 0 {
        print i * func(1, 0, a);
        i = i - 1;
    }
} else if i == 0 {
    print 10;
} else {
    print 20;
}

print a;
```



