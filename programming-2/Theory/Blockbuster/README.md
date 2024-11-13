# Blockbuster Movie Rental System

**Blockbuster** is a C++ database system designed for managing a movie rental store, similar to the classic Blockbuster video rental stores.

## Compilation

### Prerequisites
Before you can compile and use this program, make sure you have the following prerequisites installed:

* **Boost C++ Libraries (Boost.Locale):** This program relies on a portion of the Boost C++ Libraries called Boost.Locale to handle UTF-8 character manipulation. Please refer to [this guide](../../../BOOST_INSTALL.md) for detailed instructions on installing the Boost C++ Libraries.

* **Make and CMake:** You will need these build tools for compiling the program. If you don't already have them installed, please follow [this guide](../../../MAKE_CMAKE_INSTALL.md) for installing Make and CMake.

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

If you followed the instructions correctly, the compilation process should start, and once it is completed, you will be ready to use the Blockbuster Movie Rental System.

## Usage
Since the program has a terminal-based interactive interface, its usage is mostly self-explanatory. **However, it is worth noting that if the user logs in to the program as "admin", they will be able to perform admin-only tasks.**