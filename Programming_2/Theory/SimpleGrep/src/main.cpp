#include <iostream>
#include <fstream>
using std::cout, std::cerr, std::cin, std::getline, std::ifstream;

int main(int argc, char* argv[]){
    if(argc < 3){
        cerr << "Usage: sgrep [OPTION] PATTERN [FILE]\n";
        return 1;
    }

    ifstream readFile(argv[argc - 1]);
    if(!readFile){
        cerr << "sgrep: [ ERR ] FILE \"" << argv[argc - 1] << "\" DOES NOT EXIST\n";
        return 1;
    }
}