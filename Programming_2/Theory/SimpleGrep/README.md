# SimpleGrep
**An expression-matching program**

## Usage
SimpleGrep offers two operation modes. You can input both the matching expression and the target text file, or you can input only the matching expression and pipe the target text file into SimpleGrep. To run the program, open a terminal emulator or command line and use the following format:

```sgrep [OPTION...] PATTERN [FILE]```

In this format, [OPTION...] represents any optional flags or settings you can use with SimpleGrep. PATTERN stands for the matching expression you want to use, and [FILE] is an optional parameter for specifying the target text file. However, if you choose to pipe the target text file, you don't need to specify it in the program execution.

### Options
* Match color: ```-c [blue|green|cyan|red|magenta|yellow|white]```

### Examples
* File mode: ```sgrep foo ./my_file.txt```
* File mode with color option: ```sgrep -c yellow foo ./my_file.txt```
* Piped input mode: ```cat ./my_file.txt | sgrep foo```

## Compilation
To build the binary yourself, simply compile the "main.cpp" file, or execute ```make``` and ```make clean``` to build the binary using the Makefile provided.