#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
//#include <fcntl.h>              // For _setmode().
//#include <boost/locale.hpp>
#include <structs.h>
#include <file_ops.h>

#define CLTFILE_PATH "./data/clients.csv"

using   std::cout, std::cerr, std::cin, std::string, std::getline,
        std::ofstream, std::ifstream;

int main(){
    /*boost::locale::generator gen;               // Create the locale generator.
    std::locale loc = gen("en_US");             // Create an "en_US" locale
    std::locale::global(loc);                   // and set it as the global locale.
    _setmode(_fileno(stdout), _O_U8TEXT);       // Change the STDOUT mode to use UTF-8 characters.
    _setmode(_fileno(stdin), _O_U8TEXT);        // Change the STDIN mode to use UTF-8 characters.*/
    assert(sizeof(long long int) >= 8);

    int totalClients = GetFileNumOfLines(CLTFILE_PATH) - 1;

    Client baseList[totalClients + 4001];
    try{ PopulateClientList(baseList, CLTFILE_PATH); }
    catch(string exc){ cerr << exc << '\n'; return 1; }

    IntFrag ciList[totalClients + 4001];
    LLIntFrag accNumList[totalClients + 4001];
    StrFrag nameList[totalClients + 4001];

    for(int i = 1; i <= totalClients; i++){
        ciList[i].ID        = baseList[i].ID;
        ciList[i].data      = baseList[i].CI;

        accNumList[i].ID    = baseList[i].ID;
        accNumList[i].data  = baseList[i].accNum;

        nameList[i].ID      = baseList[i].ID;
        nameList[i].data    = baseList[i].name;
    }
}