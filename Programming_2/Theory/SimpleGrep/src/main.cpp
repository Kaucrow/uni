#include <iostream>
#include <fstream>
#include <map>
#ifdef _WIN32
    #include <windows.h>
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
using   std::cout, std::cerr, std::cin, std::string, 
        std::getline, std::ifstream, std::map;

enum Colors{BLUE=9, GREEN=10, CYAN=11, RED=12, MAGENTA=13, YELLOW=14, WHITE=15};
bool pipedMode = true;
Colors markColor = RED;
Colors whiteColor = WHITE;
map<string, Colors> winColorMap = {{"blue", BLUE},{"green", GREEN},{"cyan", CYAN},{"red", RED},
                                   {"magenta", MAGENTA},{"yellow", YELLOW},{"white", WHITE}};
class Exception{
    public:
        Exception(const char* setErrMsg) : errMsg(setErrMsg) {};
        Exception(const char* setErrMsg, const char* setBadColor){
            string tempStr = setErrMsg;
            tempStr = tempStr.substr(0, tempStr.find('$')) + setBadColor + tempStr.substr(tempStr.find('}') + 1);
            cout << tempStr << '\n';
            errMsg = tempStr.c_str();
        };
        void what(){ cerr << errMsg << '\n'; }
    private:
        const char* errMsg;
};

void CheckArgs(int argc, char* argv[]);
void SearchForExp(string& exp, int expLen, string& readingLine, int readLnLen);
int main(int argc, char* argv[]){
    try{ CheckArgs(argc, argv); }
    catch(Exception& exc){ exc.what(); return 1; }

    string readingLine;

    // if the binary is executed with one arg (minus options), assume that input is piped
    if(pipedMode){
        cout << "PIPED" << '\n';    // debug
        string exp(argv[argc - 1]);
        while(getline(cin, readingLine)){
            SearchForExp(exp, static_cast<int>(exp.length()), readingLine, static_cast<int>(readingLine.length()));
        }
        return 0;
    }

    // if the binary is executed with more than 2 args (minus options), assume that the last arg
    // is the file to read, and the one before it is the expression
    else{
        cout << "FILE" << '\n';     // debug
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
                SetConsoleTextAttribute(hConsole, markColor);
                cout << readingLine.substr(i, expLen);

                lineWritePos = i + expLen;
                SetConsoleTextAttribute(hConsole, whiteColor);
            }
        }
    }
    if(foundAny) cout << readingLine.substr(lineWritePos) << '\n';
    else cout << readingLine << '\n';
}

void CheckArgs(int argc, char* argv[]){
    int minArgs = 2;
    for(int i = 0; i < argc; i++){
        if(((argv[i]))[0] == '-'){
            switch(((argv[i]))[1]){
                case 'c': minArgs += 2; break;
            }
        }
    }

    if(argc < minArgs || argc > minArgs + 1){
        Exception badUsage("Usage: sgrep [OPTION...] PATTERN [FILE]"); throw badUsage; }
    else if(argc == minArgs + 1){ pipedMode = false; }

    for(int i = 0; i < argc - 1; i++){
        if(((argv[i])[0]) == '-'){
            switch(((argv[i]))[1]){
                case 'c':{
                    markColor = winColorMap.find(argv[i + 1])->second;
                    if(markColor == 0){
                        Exception badColor("sgrep: Option -c: \"${COLOR}\" is not a valid color.", argv[i + 1]); throw badColor; }
                    break;
                }
            }
        }
    }
}