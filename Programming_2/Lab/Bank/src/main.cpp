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
#include <search_ops.h>
#include <transaction.h>

#define CLTFILE_PATH "./data/clients.csv"
#define OPSFILE_PATH "./data/client_ops.csv"

using   std::cout, std::cerr, std::cin, std::string, std::getline,
        std::ofstream, std::ifstream, std::setw, std::setfill;

enum { SEARCH = 1, TRANSACTIONS = 2, EXIT = 3 };      // Actions.

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
    
    {
        ifstream clientOpsFile(OPSFILE_PATH);
        if(!clientOpsFile){
            ofstream clientOpsFile(OPSFILE_PATH, std::ios::app);
            clientOpsFile << "ci,client,balance,last_op\n";
            for(int i = 1; i <= totalClients; i++){
                clientOpsFile << setfill('0') << setw(8) << baseList[i].CI << ','
                            << baseList[i].name << ",0.0,\n";
            }
        }
        else{
            clientOpsFile.close();
            PopulateClientListBalance(baseList, OPSFILE_PATH);
        }
    }

    int action;
    int currUser = 1;
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
                    << "(2) Peform transactions\n"
                    << "(3) Exit\n"
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
                        << "(1) Search by C.I.\n"
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
                
                int matchID;
                ClrScr();
                switch(action){
                    case 1:
                        cout << "C.I. to search for: ";
                        cin >> intSearch;
                        cin.ignore(1);
                        matchID = BinSearch(ciList, 1, totalClients, intSearch);
                        if(matchID != -1) matchID = ciList[matchID].ID;
                        break;
                    case 2:
                        cout << "Account number to search for: ";
                        cin >> llintSearch;
                        cin.ignore(1);
                        matchID = BinSearch(accNumList, 1, totalClients, llintSearch);
                        if(matchID != -1) matchID = accNumList[matchID].ID;
                        break;
                    case 3:
                        cout << "Client name to search for: ";
                        getline(cin, strSearch);
                        matchID = BinSearch(nameList, 1, totalClients, strSearch);
                        if(matchID != -1) matchID = nameList[matchID].ID;
                        break;
                }

                ClrScr();
                if(matchID != -1){
                    cout    << "*** FOUND CLIENT ***\n"
                            << "-> Name: " << baseList[matchID].name << '\n'
                            << "-> C.I.: " << setfill('0') << setw(8) << baseList[matchID].CI << '\n'
                            << "-> Account number: " << setfill('0') << setw(10) << baseList[matchID].accNum << '\n'
                            << "-> Account type: " << baseList[matchID].accType << '\n'
                            << "-> Account status: ";
                    (baseList[matchID].suspended == true) ? cout << "Suspended\n" : cout << "Active\n";
                }
                else{
                    cout << "*** FOUND NO MATCHES ***\n";
                }
                cin.get();
            }
            else if(action == TRANSACTIONS){
                ClrScr();
                cout    << "*** TRANSACTION TYPE ***\n"
                        << "(1) Deposit\n"
                        << "(2) Withdraw\n"
                        << "(3) Transfer\n"
                        << "Select option: ";

                cin >> action;
                while(action < 1 || action > 3){
                    cout << "INVALID OPTION.\nSelect option: ";
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    cin >> action;
                }
                cin.ignore(1);

                string amount;
                string transferToName;
                int transferToID;
                ClrScr();
                switch(action){
                    case 1:
                        cout << "Deposit amount: ";
                        cin >> amount;
                        cin.ignore(1);
                        Deposit(OPSFILE_PATH, baseList, currUser, amount);
                        break;
                    case 2:
                        cout << "Withdraw amount: ";
                        cin >> amount;
                        cin.ignore(1);
                        try{ Withdraw(OPSFILE_PATH, baseList, currUser, amount); }
                        catch(string exc){ cerr << exc; cin.get(); }
                        break;
                    case 3:
                        cout << "Name of the beneficiary: ";
                        getline(cin, transferToName);
                        transferToID = BinSearch(nameList, 1, totalClients, transferToName);
                        if(transferToID == -1){
                            cerr << "[ ERR ] The beneficiary does not exist.\n";
                            cin.get();
                            break;
                        }
                        else{
                            transferToID = nameList[transferToID].ID;
                            if((baseList[transferToID].suspended)){ 
                                cerr << "[ ERR ] The beneficiary account is suspended.\n";
                                cin.get();
                                break;
                            }
                            else{
                                cout << "Transfer amount: ";
                                cin >> amount;
                                cin.ignore(1);
                                try{ Transfer(OPSFILE_PATH, baseList, currUser, transferToID, amount); }
                                catch(string exc){ cerr << exc; cin.get(); }
                                break;
                            }
                        } 
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