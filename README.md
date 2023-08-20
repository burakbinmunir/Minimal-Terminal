# Minimal Terminal
This project is a simple shell-like command interpreter implemented in C++.

## Features
- Supports execution of single commands and command pipelines.
- Handles input and output redirection using `<` and `>` operators.
- Implements the "cd" command for changing the working directory.
- Utilizes multiple processes, pipes, and file descriptors for command execution.

## Usage
1. Compile the program using a C++ compiler: `g++ filename.cpp -o shell`
2. Run the compiled program: `./shell`
3. Enter commands at the "$ " prompt and press Enter to execute.
4. Enter "exit" to terminate the program.

## Example Commands
- `ls -l`: Executes the "ls" command with the "-l" flag.
- `ls | wc`: Executes the "ls" command and pipes its output to the "wc" command.
- `ls > output.txt`: Executes the "ls" command and redirects its output to a file named "output.txt".
- `cat < input.txt`: Executes the "cat" command and takes input from a file named "input.txt".
- `cd path`: Changes the current working directory to "path".

## Note
This is a basic shell implementation intended for educational purposes.
