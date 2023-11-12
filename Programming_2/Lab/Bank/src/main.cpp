#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
#include <cassert>
#include <sstream>
#include <structs.h>
#include <file_ops.h>
#include <merge_sort.h>
#include <search_ops.h>
#include <transaction.h>

#define CLTFILE_PATH "./data/clients.csv"
#define OPSFILE_PATH "./data/client_ops.csv"
#define TTLFILE_PATH "./data/title.txt"

using   std::cout, std::cerr, std::cin, std::string, std::getline,
        std::ofstream, std::ifstream, std::setw, std::setfill;

enum { SEARCH = 1, TRANSACTIONS = 2, SUSPEND = 3, EXIT = 4 };       // Actions.

/**
 * @brief OS agnostic clear screen function.
 */
void ClrScr();

int main(){
    assert(sizeof(long long int) >= 8);

    // Get the number of clients in the clients.csv file. //
    int totalClients = GetFileNumOfLines(CLTFILE_PATH) - 1;

    // Create a base list of clients (sorted by ID), where the first element is unused. //
    Client baseList[totalClients + 1];

    // Populate the base client list. //
    try{ PopulateClientList(baseList, CLTFILE_PATH); }
    catch(string exc){ cerr << exc; return 1; }

    // ==================
    // Create lists of client fragments, where the first element of the array is unused.
    // Each frag list is sorted according to the client data it contains
    // (C.I., account number, client name).
    // ==================
    IntFrag ciList[totalClients + 1];
    LLIntFrag accNumList[totalClients + 1];
    StrFrag nameList[totalClients + 1];

    // Copy the base client list elements data to each frag list. //
    for(int i = 1; i <= totalClients; i++){
        ciList[i].ID        = baseList[i].ID;
        ciList[i].data      = baseList[i].CI;

        accNumList[i].ID    = baseList[i].ID;
        accNumList[i].data  = baseList[i].accNum;

        nameList[i].ID      = baseList[i].ID;
        nameList[i].data    = baseList[i].name;
    }

    // Sort each frag list. //
    MergeSort(ciList, 1, totalClients);
    MergeSort(accNumList, 1, totalClients);
    MergeSort(nameList, 1, totalClients);

    // Check whether the client_ops.csv file has been created. //
    {
        ifstream clientOpsFile(OPSFILE_PATH);
        // If it hasn't been created, create it. //
        if(!clientOpsFile){
            ofstream clientOpsFile(OPSFILE_PATH, std::ios::app);
            clientOpsFile << "ci,client,balance,last_op\n";
            for(int i = 1; i <= totalClients; i++){
                clientOpsFile << setfill('0') << setw(8) << baseList[i].CI << ','
                            << baseList[i].name << ",0.0,\n";
            }
        }
        // If it has already been created, set the clients'
        // balance according to the data in the file.
        else{
            clientOpsFile.close();
            PopulateClientListBalance(baseList, OPSFILE_PATH);
        }
    }

    // Display the program title. //
    ClrScr();
    {
        string readingLine;
        ifstream titleFile(TTLFILE_PATH);
        if(!titleFile){ cerr << "[ ERR ] title.txt COULD NOT BE OPENED.\n"; return 1; }
        while(getline(titleFile, readingLine)){
            cout << readingLine << '\n';
        }
    }
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    // =========================
    //  Main loop.
    // =========================
    int action;
    int currClient;       // ID of the active client.
    string clientName;    // Full name of the active client.
    while(true){
        // Main menu screen. //
        ClrScr();
        cout    << "*** MENU ***\n"
                << "(1) Login\n"
                << "(2) Exit\n"
                << "Select option: ";
        cin >> action;
        switch(action){
            case 1: break;
            case 2:
                cout << "\nTerminating execution...\n";
                return 0;
        }
        cin.ignore(1);

        // Login screen. //
        ClrScr();
        cout    << "*** LOGIN ***\n"
                << "-> Client full name: ";
        getline(cin, clientName);

        // Get the client's index in the nameList array. //
        currClient = BinSearch(nameList, 1, totalClients, clientName);

        // If the client doesn't exist, output an error message. //
        if(currClient == -1){
            cerr << "[ ERR ] The client could not be found.\n";
            cin.get();
            continue;
        }

        // Set the actual client ID. //
        currClient = nameList[currClient].ID;

        // If the client's account is suspended, output an error message. //
        if(baseList[currClient].suspended){
            cerr << "[ INFO ] This account is currently suspended.\n";
            cin.get();
            continue;
        }

        // If the client's name and ID were set properly, display the main actions screen. //
        while(true){
            // Main actions screen. //
            ClrScr();
            cout    << "*** CHOOSE AN ACTION ***\n"
                    << "(1) Search for a client\n"
                    << "(2) Peform transactions\n"
                    << "(3) Suspend current user\n"
                    << "(4) Exit\n"
                    << "Select option: ";

            cin >> action;
            while(action < SEARCH || action > EXIT){
                cout << "INVALID OPTION.\nSelect option: ";
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cin >> action;
            }
            cin.ignore(1);

            //=========================
            //  Search for a client.
            // ========================
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
                
                int matchID;        // ID of the matched client.
                ClrScr();
                switch(action){
                    case 1:
                        cout << "C.I. to search for: ";
                        cin >> intSearch;
                        cin.ignore(1);
                        matchID = BinSearch(ciList, 1, totalClients, intSearch);  // Get the client's index in the ciList array.
                        if(matchID != -1) matchID = ciList[matchID].ID;     // If the client was found, set its actual ID.
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
                // If the client was found, display its information. //
                if(matchID != -1){
                    cout    << "*** FOUND CLIENT ***\n"
                            << "-> Name: " << baseList[matchID].name << '\n'
                            << "-> C.I.: " << setfill('0') << setw(8) << baseList[matchID].CI << '\n'
                            << "-> Account number: " << setfill('0') << setw(10) << baseList[matchID].accNum << '\n'
                            << "-> Account type: ";
                    (baseList[matchID].accType == ACC_CURRENT) ? cout << "Current\n" : cout << "Debit\n";
                    cout    << "-> Account status: ";
                    (baseList[matchID].suspended == true) ? cout << "Suspended\n" : cout << "Active\n";
                }
                // If the client was not found, display a message indicating so. //
                else{
                    cout << "*** FOUND NO MATCHES ***\n";
                }
                cin.get();
            }
            //=========================
            //  Perform a transaction.
            // ========================
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

                string amount;              // Amount of money to be deposited/withdrawn/transferred.
                string transferToName;      // Name of transfer beneficiary.
                int transferToID;           // ID of the transfer beneficiary.
                ClrScr();
                switch(action){
                    // Deposit. //
                    case 1:
                        cout << "Deposit amount: ";
                        cin >> amount;
                        cin.ignore(1);
                        Deposit(OPSFILE_PATH, baseList, currClient, amount);
                        break;
                    // Withdraw. //
                    case 2:
                        cout << "Withdraw amount: ";
                        cin >> amount;
                        cin.ignore(1);
                        try{ Withdraw(OPSFILE_PATH, baseList, currClient, amount); }
                        catch(string exc){ cerr << exc; cin.get(); }
                        break;
                    // Transfer. //
                    case 3:
                        cout << "Name of the beneficiary: ";
                        getline(cin, transferToName);

                        // Get the transfer beneficiary index on the nameList. //
                        transferToID = BinSearch(nameList, 1, totalClients, transferToName);

                        // If the transfer beneficiary was not found, output an error message. //
                        if(transferToID == -1){
                            cerr << "[ ERR ] The beneficiary does not exist.\n";
                            cin.get();
                            break;
                        }
                        else{
                            transferToID = nameList[transferToID].ID;
                            // If the transfer beneficiary is a suspended account, output an error message. //
                            if((baseList[transferToID].suspended)){ 
                                cerr << "[ ERR ] The beneficiary account is suspended.\n";
                                cin.get();
                                break;
                            }
                            // Executes if the transfer can be done to the entered beneficiary. //
                            else{
                                cout << "Transfer amount: ";
                                cin >> amount;
                                cin.ignore(1);
                                try{ Transfer(OPSFILE_PATH, baseList, currClient, transferToID, amount); }
                                catch(string exc){ cerr << exc; cin.get(); }
                                break;
                            }
                        } 
                }
            }
            //=========================
            //  Suspend the active
            //  client account.
            // ========================
            else if(action == SUSPEND){
                ClrScr();

                // Update the live base client list data. //
                baseList[currClient].suspended = true;

                // Update the clients.csv file data. //
                std::stringstream lineUpdate;
                lineUpdate  << setfill('0') << setw(8) << baseList[currClient].CI << ','
                            << baseList[currClient].name << ','
                            << setfill('0') << setw(10) << baseList[currClient].accNum << ',';
                (baseList[currClient].accType == ACC_CURRENT) ? 
                    lineUpdate  << "current," : lineUpdate << "debit,";
                lineUpdate  << "true";

                ReplaceLine(CLTFILE_PATH, lineUpdate.str(), currClient + 1);

                // Output a "success" message. //
                cout << "[ INFO ] The account has been suspended successfully.\n";
                cin.get();
                break;
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