#include <iostream>
#include <fstream>
#include <map>
#ifdef _WIN32
    #include <windows.h>
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
using   std::cout, std::cerr, std::cin, std::string, 
        std::getline, std::ifstream, std::map;

enum winColors{BLUE=9, GREEN=10, CYAN=11, RED=12, MAGENTA=13, YELLOW=14, WHITE=15};
bool pipedMode = true;
winColors markColor = RED;
winColors whiteColor = WHITE;
map<string, winColors> winColorMap = {{"blue", BLUE},{"green", GREEN},{"cyan", CYAN},{"red", RED},
                                   {"magenta", MAGENTA},{"yellow", YELLOW},{"white", WHITE}};
class Exception{
    public:
        // normal msg exception: Exception excName(Message...)
        Exception(const char* setErrMsg) : errMsg(setErrMsg) {};
        // substitution msg exception: Exception excName(Message...${SUBSTITUTE}Message..., SUBSTITUTE_VAR)
        Exception(const char* setErrMsg, const char* setBadColor){
            string tempStr = setErrMsg;
            errMsg = tempStr.substr(0, tempStr.find('$')) + setBadColor + tempStr.substr(tempStr.find('}') + 1);
        };
        void what(){ cerr << errMsg << '\n'; }
    private:
        string errMsg;
};

void CheckArgs(int argc, char* argv[]);
void SearchForExp(string& exp, int expLen, string& readingLine, int readLnLen);
int main(int argc, char* argv[]){
    // CheckArgs() checks if all arguments are correctly typed, and sets various
    // global variables according to the value of the options provided. An exception
    // is thrown if any option is invalid or there is an unexpected number of args
    try{ CheckArgs(argc, argv); }
    catch(Exception& exc){ exc.what(); return 1; }

    string readingLine;

    // if the binary is executed with one arg (minus options), assume that input is piped
    if(pipedMode){
        string exp(argv[argc - 1]);
        int expLen = static_cast<int>(exp.length());
        while(getline(cin, readingLine)){
            SearchForExp(exp, expLen, readingLine, static_cast<int>(readingLine.length()));
        }
        return 0;
    }

    // if the binary is executed with more than 2 args (minus options), assume that the last arg
    // is the file to read, and the one before it is the expression to match
    else{
        string exp(argv[argc - 2]); 
        int expLen = static_cast<int>(exp.length());

        ifstream readFile(argv[argc - 1]);
        if(!readFile){
            cerr << "sgrep: [ ERR ] FILE \"" << argv[argc - 1] << "\" DOES NOT EXIST\n";
            return 1;
        }

        while(getline(readFile, readingLine)){
            SearchForExp(exp, expLen, readingLine, static_cast<int>(readingLine.length()));
        }
        return 0;
    }
}

void SearchForExp(string& exp, int expLen, string& readingLine, int readLnLen){
    // return, since the exp can't be in the readingLine
    if(readLnLen < expLen) return;
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
                SetConsoleTextAttribute(hConsole, markColor);       // set the foreground highlight color
                cout << readingLine.substr(i, expLen);

                lineWritePos = i + expLen;
                i = lineWritePos - 1;       // - 1, since 'i' will get incremented on next for loop iteration
                SetConsoleTextAttribute(hConsole, whiteColor);      // return to normal foreground color
            }
        }
    }
    if(foundAny) cout << readingLine.substr(lineWritePos) << '\n';
}

void CheckArgs(int argc, char* argv[]){
    int minArgs = 2;    // min args that the program call requires. Increases according to type/num of options
    // find options in program call and update the num of min args accordingly
    for(int i = 0; i < argc; i++){
        if(((argv[i]))[0] == '-'){
            switch(((argv[i]))[1]){
                // color
                case 'c': minArgs += 2; break;
            }
        }
    }

    // throw an exception if the num of args provided is greater or less than expected
    if(argc < minArgs || argc > minArgs + 1){
        Exception badUsage("Usage: sgrep [OPTION...] PATTERN [FILE]"); throw badUsage; }
    else if(argc == minArgs + 1){ pipedMode = false; }  // disable piped mode if the num of args is one more than the minArgs

    // find options in program call and update global variables accordingly
    for(int i = 0; i < argc - 1; i++){
        if(((argv[i])[0]) == '-'){
            switch(((argv[i]))[1]){
                case 'c':
                    markColor = winColorMap.find(argv[i + 1])->second;
                    // throw an exception if the color wasn't found in the map of colors
                    if(markColor == 0){
                        Exception badColor("sgrep: Option -c: \"${COLOR}\" is not a valid color.", argv[i + 1]); throw badColor; }
                    break;
            }
        }
    }
}