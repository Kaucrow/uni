#include <iostream>
#include <fstream>
#ifdef _WIN32
    #include <windows.h>
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
using   std::cout, std::cerr, std::cin, std::string, 
        std::getline, std::ifstream;

void SearchForExp(string& exp, int expLen, string& readingLine, int readLnLen);
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
            SearchForExp(exp, static_cast<int>(exp.length()), readingLine, static_cast<int>(readingLine.length()));
        }
        return 0;
    }

    // if the binary is executed with more than 2 args, assume that the last arg
    // is the file to read, and the one before it is the expression
    else{
        string exp(argv[argc - 2]);

        ifstream readFile(argv[argc - 1]);
        if(!readFile){
            cerr << "sgrep: [ ERR ] FILE \"" << argv[argc - 1] << "\" DOES NOT EXIST\n";
            return 1;
        }

        while(getline(readFile, readingLine)){
            SearchForExp(exp, static_cast<int>(exp.length()), readingLine, static_cast<int>(readingLine.length()));
        }
        return 0;
    }
}

void SearchForExp(string& exp, int expLen, string& readingLine, int readLnLen){
    if(readLnLen < expLen){ cout << readingLine << '\n'; return; }
    bool foundAny = false;      // true if the exp was found anywhere in the line, false otherwise
    int lineWritePos = 0;       // stores the pos of the next character to write from the line
    for(int i = 0; i <= (readLnLen - expLen); i++){
        // check for exp match if the first character of the exp matches the character being read
        if(exp[0] == readingLine[i]){
            bool foundCurr = true;
            for(int j = 0; j < expLen; j++){
                // if any character differs, set foundCurr to false & exit loop
                if(!(exp[j] == readingLine[i + j])){ foundCurr = false; break; }
            }
            if(foundCurr){
                foundAny = true;
                cout << readingLine.substr(lineWritePos, i - lineWritePos);
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                cout << readingLine.substr(i, expLen);

                lineWritePos = i + expLen;
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
    }
    if(foundAny) cout << readingLine.substr(lineWritePos) << '\n';
    else cout << readingLine << '\n';
}