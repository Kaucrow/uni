#include "transaction.h"
#include "../file_ops/file_ops.h"

using std::to_string;

/**
 * @brief Converts a string object containing a money amount into dollars and cents int vars. 
 * @param moneyStr - Money amount string object. Example money amount string objects: 3.141592 or 100,07
 * @param dollars - Var to store the converted dollars in.
 * @param dollars - Var to store the converted cents in.
*/
void StrToMoney(string moneyStr, int& dollars, int& cents){
    int delimPos;

    delimPos = moneyStr.find('.');      // Try to find a dot delimiter.

    if(delimPos == -1) delimPos = moneyStr.find(',');       // If it couldn't be found, try to find a comma delimiter.

    // If it still couldn't be found, assume that there is no delimiter
    // and the money amount string only holds an integer value.
    if(delimPos == -1){
        dollars = stoi(moneyStr);
        cents = 0;
    // Executes if the delimiter was found. //
    } else{
        dollars = stoi(moneyStr);
        cents = stoi(moneyStr.substr(delimPos + 1, 2));     // The cents are stored with a two-decimal-places truncation.
    }
}

/**
 * @brief Function to be called by Deposit() or Transfer().
*/
void DepositCall(const char* clientOpsPath, Client baseList[], int clientID, int addDollars, int addCents){
    baseList[clientID].balance.dollars += addDollars;       // Add the dollars amount to the client.
    baseList[clientID].balance.cents += addCents;           // Add the cents amount to the client.

    // If the amount of cents is now higher or equal than 100, convert 100 cents into 1 dollar. //
    if(baseList[clientID].balance.cents >= 100){
        baseList[clientID].balance.dollars++;
        baseList[clientID].balance.cents -= 100;
    }

    // Update the client_ops.csv data. //
    string lineUpdate = to_string(baseList[clientID].CI) + ',' + baseList[clientID].name + ',' + 
                        to_string(baseList[clientID].balance.dollars) + '.' + to_string(baseList[clientID].balance.cents) + ",+" + 
                        to_string(addDollars) + '.' + to_string(addCents);
    ReplaceLine(clientOpsPath, lineUpdate, clientID + 1);
}

void Deposit(const char* clientOpsPath, Client baseList[], int clientID, string amount){
    int addDollars = 0, addCents = 0;
    StrToMoney(amount, addDollars, addCents);
    DepositCall(clientOpsPath, baseList, clientID, addDollars, addCents);
}

/**
 * @brief Function to be called by Withdraw() or Transfer().
*/
void WithdrawCall(const char* clientOpsPath, Client baseList[], int clientID, int withdrawDollars, int withdrawCents){
    // Check if the withdrawal can be performed. If it can't, throw an exception. //
    if(withdrawDollars >= baseList[clientID].balance.dollars){
        string notEnoughMoney("[ ERR ] You don't have enough money to perform this withdrawal.\n");
        if(withdrawDollars > baseList[clientID].balance.dollars) throw notEnoughMoney;
        else if(withdrawCents > baseList[clientID].balance.cents ) throw notEnoughMoney;
    }

    baseList[clientID].balance.dollars -= withdrawDollars;      // Take the dollars amount from the client.
    baseList[clientID].balance.cents -= withdrawCents;          // Take the cents amount from the client.

    //If the amount of cents is now negative, convert 100 cents to 1 dollar.
    if(baseList[clientID].balance.cents < 0){
        baseList[clientID].balance.dollars--;
        baseList[clientID].balance.cents += 100;
    }

    // Update the client_ops.csv data. //
    string lineUpdate = to_string(baseList[clientID].CI) + ',' + baseList[clientID].name + ',' + 
                        to_string(baseList[clientID].balance.dollars) + '.' + to_string(baseList[clientID].balance.cents) + ",-" + 
                        to_string(withdrawDollars) + '.' + to_string(withdrawCents);
    ReplaceLine(clientOpsPath, lineUpdate, clientID + 1);
}

void Withdraw(const char* clientOpsPath, Client baseList[], int clientID, string amount){
    int withdrawDollars = 0, withdrawCents = 0;
    StrToMoney(amount, withdrawDollars, withdrawCents);
    try{ WithdrawCall(clientOpsPath, baseList, clientID, withdrawDollars, withdrawCents); }
    catch(string exc){ throw; }
}

void Transfer(const char* clientOpsPath, Client baseList[], int clientFromID, int clientToID, string amount){
    int transferDollars = 0, transferCents;
    StrToMoney(amount, transferDollars, transferCents);

    // Perform a withdrawal to the account the money is being transferred from. //
    try{ WithdrawCall(clientOpsPath, baseList, clientFromID, transferDollars, transferCents); }
    catch(string exc){ exc = "[ ERR ] You don't have enough money to perform this transfer.\n"; throw exc; } 

    // Perform a deposit to the account the money is being transferred to. //
    DepositCall(clientOpsPath, baseList, clientToID, transferDollars, transferCents);

    // Update the client_ops.csv data. //
    string lineUpdate = to_string(baseList[clientFromID].CI) + ',' + baseList[clientFromID].name + ',' + 
                        to_string(baseList[clientFromID].balance.dollars) + '.' + to_string(baseList[clientFromID].balance.cents) + ',' + 
                        to_string(transferDollars) + '.' + to_string(transferCents) + "->" + baseList[clientToID].name;
    ReplaceLine(clientOpsPath, lineUpdate, clientFromID + 1);
}