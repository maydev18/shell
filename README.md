
# Custom Shell in C

  

A lightweight Unix shell implemented in C that supports command execution, piping, background processes, redirection, and inter-process communication(IPC) using `fork()`, `execvp()`, and `pipe()`.

  

## Features

  

- Executes basic shell commands (e.g., `ls`, `pwd`, `echo`, etc)

- Supports multiple piping (e.g., `ls -l | grep .c | sort`)

- Handles input/output redirection (`<`, `>`)

- Implements inter-process communication using `pipe()`

- Handles process creation and termination, using `fork()` and `wait()` syscalls

  

## Getting Started

  

### Prerequisites

- GCC compiler

- Unix/Linux environment

  

### Compile
- option1
```bash
$> make
```
- Option2
```bash
$> gcc main.c shell.c utils.c -o myshell
```
  

### Run

- option1
```bash
$ make run
```
- Option2
```bash
$ ./myshell
```

  

## **Usage Examples**

- To list all files and directories in long format
```	
$ ls -l
```
- To list all files and directories recursively
```	
$ ls -R
```
- To get current working directory
```	
$ pwd
```
- To copy the contents of input.txt to output.txt
```	
$ cat < input.txt > output.txt
```
- To count the number of characters in a given input
```	
$ echo mayank | wc -c
```
- To List top 5 files(in terms of size) in the pwd such that they are sorted in lexicographical order 
```	
$ ls -lhS | sort -k9 | head -n 5    
```
- To run a background process, such that after running it, the shell immediately returns to user and print its `process id`
```	
$ sleep 5 &
```
## Code Structure
```
├── docs/ // Document Folder
├── main.c // Entry point
├── shell.h // Header file for shell
├── shell.c // Contains definitions of functions
├── Utils.h //Header file for utility functions
├── Utils.c //contains definitions of utility functions
├── colors.h //contains ANSI standard escape sequences for colors
├── Makefile // Build automation
├── doxyfile //doxyfile for docs generation
├── input.txt // A sample input file for some commands
└── README.md
```

## Documentation
[View Documentation](https://maydev18.github.io/shell/)

All major functions are documented with inline comments.  
For deeper technical insights, explore the `.c` and `.h` files directly.


## Future Improvements

- Built-in command support (like `cd`, `exit`)
- Listening to `arrow keys` for generation of previous commands and maintaining command history
- Integrate arithmetic parser


## Author and License
```
## Author

Made by Mayank Sharma

## License

MIT License
```
