#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
#include <cassert>
//#include <fcntl.h>              // For _setmode().
//#include <boost/locale.hpp>
#include <structs.h>
#include <file_ops.h>
#include <merge_sort.h>

#define CLTFILE_PATH "./data/clients.csv"

using   std::cout, std::cerr, std::cin, std::string, std::getline,
        std::ofstream, std::ifstream;

enum { SEARCH = 1, EXIT = 2 };      // Actions.

/**
 * @brief OS agnostic clear screen function.
 */
void ClrScr();

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

    MergeSort(ciList, 1, totalClients);
    MergeSort(accNumList, 1, totalClients);
    MergeSort(nameList, 1, totalClients);

    int action;
    string username;
    while(true){
        // Main menu screen. //
        ClrScr();
        cout    << "*** MENU ***\n"
                << "(1) Login\n"
                << "(2) Exit\n"
                << "Select option: ";
        //cin >> action;
        switch(action){
            case 1: break;
            case 2:
                cout << "\nTerminating execution...\n";
                return 0;
        }
        //cin.ignore(1);

        // Login screen. //
        ClrScr();
        cout    << "*** LOGIN ***\n"
                << "-> User: ";
        //getline(cin, username);

        while(true){
            // Main actions screen. //
            ClrScr();
            cout    << "*** CHOOSE AN ACTION ***\n"
                    << "(1) Search for a client\n"
                    << "(2) Exit\n"
                    << "Select option: ";

            cin >> action;
            while(action < SEARCH || action > EXIT){
                cout << "INVALID OPTION.\nSelect option: ";
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> action;
            }
            cin.ignore(1);

            if(action == SEARCH){
                ClrScr();
                cout    << "*** CLIENT SEARCH ***\n"
                        << "(1) Search by CI\n"
                        << "(2) Search by account number\n"
                        << "(3) Search by client name\n"
                        << "Select option: ";

                cin >> action;
                while(action < 1 || action > 3){
                    cout << "INVALID OPTION.\nSelect option: ";
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cin >> action;
                }
                cin.ignore(1);

                int intSearch;
                long long int llintSearch;
                string strSearch;
                ClrScr();
                switch(action){
                    case 1:
                        cout << "CI to search for: ";
                        cin >> intSearch;
                        cin.ignore(1);
                        break;
                    case 2:
                        cout << "Account number to search for: ";
                        cin >> llintSearch;
                        cin.ignore(1);
                        break;
                    case 3:
                        cout << "Client name to search for: ";
                        getline(cin, strSearch);
                        break;
                } 
            }
            else break;
        }
    }
}

void ClrScr(){
    #ifdef _WIN32
        // If on Windows OS
        std::system("cls");
    #else
        // Assuming POSIX OS
        std::system("clear");
    #endif
}