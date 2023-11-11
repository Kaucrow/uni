#include "transaction.h"
#include "../file_ops/file_ops.h"

using std::to_string;

void StrToMoney(string moneyStr, int& dollars, int& cents){
    int delimPos;

    delimPos = moneyStr.find('.');

    if(delimPos == -1) delimPos = moneyStr.find(',');

    if(delimPos == -1){
        dollars = stoi(moneyStr);
        cents = 0;
    } else{
        dollars = stoi(moneyStr);
        cents = stoi(moneyStr.substr(delimPos + 1, 2));
    }
}

void Deposit(const char* clientOpsPath, Client baseList[], int userID, string amount){
    int addDollars = 0, addCents = 0;
    StrToMoney(amount, addDollars, addCents);
    baseList[userID].balance.dollars += addDollars;
    baseList[userID].balance.cents += addCents;
    if(baseList[userID].balance.cents >= 100){
        baseList[userID].balance.dollars++;
        baseList[userID].balance.cents -= 100;
    }

    string lineUpdate = to_string(baseList[userID].CI) + ',' + baseList[userID].name + ',' + 
                        to_string(baseList[userID].balance.dollars) + '.' + to_string(baseList[userID].balance.cents) + ",+" + 
                        to_string(addDollars) + '.' + to_string(addCents);
    ReplaceLine(clientOpsPath, lineUpdate, userID + 1);
}

void Withdraw(const char* clientOpsPath, Client baseList[], int userID, string amount){
    int withdrawDollars = 0, withdrawCents = 0;
    StrToMoney(amount, withdrawDollars, withdrawCents);

    if(withdrawDollars >= baseList[userID].balance.dollars){
        string notEnoughMoney("[ ERR ] You don't have enough money to perform this withdrawal.\n");
        if(withdrawDollars > baseList[userID].balance.dollars) throw notEnoughMoney;
        else if(withdrawCents > baseList[userID].balance.cents ) throw notEnoughMoney;
    }

    baseList[userID].balance.dollars -= withdrawDollars;
    baseList[userID].balance.cents -= withdrawCents;
    if(baseList[userID].balance.cents < 0){
        baseList[userID].balance.dollars--;
        baseList[userID].balance.cents += 100;
    }

    string lineUpdate = to_string(baseList[userID].CI) + ',' + baseList[userID].name + ',' + 
                        to_string(baseList[userID].balance.dollars) + '.' + to_string(baseList[userID].balance.cents) + ",-" + 
                        to_string(withdrawDollars) + '.' + to_string(withdrawCents);
    ReplaceLine(clientOpsPath, lineUpdate, userID + 1);
}

void Transfer(const char* clientOpsPath, Client baseList[], int userFromID, int userToID, string amount){
    int transferDollars = 0, transferCents;
    StrToMoney(amount, transferDollars, transferCents);

    try{ Withdraw(clientOpsPath, baseList, userFromID, amount); }
    catch(string exc){ exc = "[ ERR ] You don't have enough money to perform this transfer.\n"; throw exc; } 
    Deposit(clientOpsPath, baseList, userToID, amount);

    string lineUpdate = to_string(baseList[userFromID].CI) + ',' + baseList[userFromID].name + ',' + 
                        to_string(baseList[userFromID].balance.dollars) + '.' + to_string(baseList[userFromID].balance.cents) + ',' + 
                        to_string(transferDollars) + '.' + to_string(transferCents) + "->" + baseList[userToID].name;
    ReplaceLine(clientOpsPath, lineUpdate, userFromID + 1);
}