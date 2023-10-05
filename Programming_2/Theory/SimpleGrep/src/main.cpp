#include <iostream>
#include <fstream>
#ifdef _WIN32
    #include <windows.h>
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
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
    if(static_cast<int>(readingLine.length()) < static_cast<int>(exp.length())){ cout << readingLine << '\n'; return; }
    bool foundCurr = true;
    bool foundAny = false;
    int lastMatchPos = 0;
    for(int i = 0; i <= (static_cast<int>(readingLine.length()) - static_cast<int>(exp.length())); i++){
        if(exp[0] == readingLine[i]){
            for(int j = 0; j < exp.length(); j++){
                if(!(exp[j] == readingLine[i + j])){ foundCurr = false; break; }
            }
            if(foundCurr){
                foundAny = true;
                cout << readingLine.substr(lastMatchPos, i - lastMatchPos);
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << readingLine.substr(i, static_cast<int>(exp.length()));

                lastMatchPos = i + static_cast<int>(exp.length());
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            else{ foundCurr = true; }
        }
    }
    if(foundAny) cout << readingLine.substr(lastMatchPos) << '\n';
    else cout << readingLine << '\n';
}