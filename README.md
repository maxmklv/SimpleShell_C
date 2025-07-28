# Project 1: Simple Shell — Basic C Shell Implementation

This project focuses on the design and implementation of a simple shell in C. The goal is to apply C programming concepts to recreate basic shell functionality and establish a foundation for future projects. The program provides:

- **Command Parsing and Execution**: Supports core shell operations  
- **Exit Functionality**: Clean termination of the shell  
- **System Information Access**: Reads from the `/proc` filesystem  
- **Command History**: Maintains a history of executed commands  

The shell updates a `.421sh` file to store the command history.

## How to Build?

The project uses standard C libraries and `utils.h`. A provided `Makefile` handles the build process.

```bash
# Step 1: Compile the project
make

# Step 2: Run the shell
bash ./main

```

# References

## Authors
Maxim Mikhaylov
- [github](https://www.github.com/maxmklv/SimpleShell_C)
