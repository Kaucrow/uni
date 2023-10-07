#include <iostream>
#include <fstream>
#include <fcntl.h>              // for _setmode()
#include <boost/locale.hpp>
#include "structs.h"
using   std::cout, std::cerr, std::cin, std::string, std::getline,
        std::wifstream, std::wifstream, std::wstring, std::wcout;

int GetNumMovies(wifstream& inFile);
void PopulateMovieList(Movie movieList[], wifstream& inFile);
int main(){
    boost::locale::generator gen;
    std::locale loc = gen("en_US");
    std::locale::global(loc);
    _setmode(_fileno(stdout), _O_U8TEXT);

    string inFileName = "./data/movies.csv";
    wifstream inFile(inFileName);
    inFile.imbue(loc);
    if(!inFile){ cerr << "ERR: FILE \"" << inFileName << "\" COULD NOT BE OPENED."; return 1; }

    Movie movieList[GetNumMovies(inFile) + 3001];
    wcout << GetNumMovies(inFile) << '\n';
    wcout << sizeof(movieList) << '\n';
    //PopulateMovieList(movieList, inFile);
}

/*void PopulateMovieList(Movie movieList[], wifstream& inFile){
    int numMovies = GetNumMovies(inFile);
    string readingLine;
    std::wstring wideReadingLine;
    int commaPos[6];
    auto GetCommas = [&wideReadingLine, &commaPos](){
        int nextComma, totalComma = 0;
        for(int i = 0; i < 6; i++){
            nextComma = wideReadingLine.find(',');
            if(i == 0) totalComma += nextComma;
            else totalComma += nextComma + 1;
            commaPos[i] = totalComma;
            std::wcout << wideReadingLine << '\n';
            wideReadingLine = wideReadingLine.substr(nextComma + 1);
        }
    };
    inFile.seekg(0);
    getline(inFile, wideReadingLine);

    for(int i = 1; i <= numMovies; i++){
        getline(inFile, wideReadingLine);
        if(i == 1) GetCommas();
        // debug
        if(i == 1){
            cout << commaPos[0] << '\n';
            cout << commaPos[1] << '\n';
            cout << commaPos[2] << '\n';
            cout << commaPos[3] << '\n';
            cout << commaPos[4] << '\n';
            cout << commaPos[5] << '\n';
        }
        //movieList[i].ID = stoi(readingLine.substr(0, readingLine.find(',')));
    }
}*/

int GetNumMovies(wifstream& inFile){
    inFile.seekg(0, std::ios_base::end);        // end:  74182
    std::wifstream::pos_type pos = inFile.tellg();
    pos = int(pos) - 1;
    inFile.seekg(pos);
    std::wcout << pos << '\n';

    wchar_t ch;
    while(ch != L'\n'){
        pos = int(pos) - 1;
        inFile.seekg(pos);
        inFile.get(ch);
        std::wcout << ch << ' ';
    }
    wstring lastLine;
    getline(inFile, lastLine);
    wcout << lastLine << '\n';

    inFile.seekg(0, std::ios_base::beg);
    return stoi(lastLine);
}