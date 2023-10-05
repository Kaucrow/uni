#include <iostream>
#include <fstream>
using   std::cout, std::cerr, std::cin, std::string, 
        std::getline, std::ifstream;

void SearchForExp(string exp, string readingLine);
int main(int argc, char* argv[]){
    string readingLine; 

    // if the binary is executed without args, throw an error
    if(argc == 1){
        cerr << "Usage: sgrep [OPTION] PATTERN [FILE]\n";
        return 1;
    }

    // if the binary is executed with one arg, assume that input is piped
    else if(argc == 2){
        string exp(argv[argc - 1]);
        cout << "PIPED INPUT\n";
        while(getline(cin, readingLine)){
            SearchForExp(exp, readingLine);
        }
        return 0;
    }

    string exp(argv[argc - 2]);

    // if the binary is executed with more than 2 args, get the file to read
    ifstream readFile(argv[argc - 1]);
    if(!readFile){
        cerr << "sgrep: [ ERR ] FILE \"" << argv[argc - 1] << "\" DOES NOT EXIST\n";
        return 1;
    }

    while(getline(readFile, readingLine)){
        SearchForExp(exp, readingLine);
    }
    return 0; 
}

void SearchForExp(string exp, string readingLine){
    bool found = true;
    cout << readingLine.length() << '\n';
    cout << readingLine << '\n';
    cout << exp.length() << '\n';
    cout << exp << '\n';
    for(int i = 0; i <= (static_cast<int>(readingLine.length()) - static_cast<int>(exp.length())); i++){
        if(exp[0] == readingLine[i]){
            for(int j = 0; j < exp.length(); j++){
                if(!(exp[j] == readingLine[i + j])){ found = false; break; }
            }
            if(found){ cout << "FOUND" << '\n'; }
            else{ found = true; }
        }
    }
}