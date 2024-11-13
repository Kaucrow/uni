# Bank Account Management System

**Bank** is a C++ database system designed for managing the client accounts of an imaginary bank.

## Compilation

### Prerequisites
Before you can compile and use this program, make sure you have the following prerequisites installed:

* **Make and CMake:** You will need these build tools for compiling the program. If you don't already have them installed, please follow [this guide](../../../MAKE_CMAKE_INSTALL.md) for installing Make and CMake on Windows.

### Compiling on Windows
To compile the program, follow these simple steps:

1. Open a terminal or the Windows Command Prompt and cd into the folder containing the project.

2. Execute the "compile.bat" script by running the following command:

        ./compile.bat

### Compiling on Linux
To compile the program, follow these simple steps:

1. Open a terminal and cd into the folder containing the project.

2. Set the execution flag of the "compile.sh" file: `chmod +x compile.sh`

3. Execute the "compile.sh" script by running the following command:
    
        ./compile.sh

<hr>

If you followed the instructions correctly, the compilation process should start, and once it is completed, you will be ready to use the Bank Account Management System.

## Usage
Since the program has a terminal-based interactive interface, its usage is mostly self-explanatory. **However, it is worth noting that if the user logs in to the program as "admin", they will be able to perform admin-only tasks.**