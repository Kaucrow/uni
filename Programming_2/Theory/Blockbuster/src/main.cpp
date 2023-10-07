#include <iostream>
#include <fstream>
using   std::cout, std::cerr, std::cin, std::string, std::getline,
        std::ifstream, std::ofstream, std::fstream;

int GetNumMovies(ifstream& inFile);
int main(){
    int numMovies;
    string inFileName = "./data/movies.csv";
    ifstream inFile(inFileName);
    if(!inFile){ cerr << "ERR: FILE \"" << inFileName << "\" COULD NOT BE OPENED."; return 1; }
    
    numMovies = GetNumMovies(inFile);
}

int GetNumMovies(ifstream& inFile){
    inFile.seekg(-1, std::ios_base::end);   // go to one spot before the EOF
    
    char ch = ' ';
    while(ch != '\n'){
        cout << ch << ' ';
        inFile.seekg(-2, std::ios_base::cur);
        inFile.get(ch);
    }

    string lastLine;
    getline(inFile, lastLine);
    cout << "RESULT: " << lastLine << '\n';     // debug
    inFile.seekg(0);
    return stoi(lastLine);
    return 0;
}