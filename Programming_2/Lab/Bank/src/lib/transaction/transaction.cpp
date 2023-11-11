#include "transaction.h"
#include "../file_ops/file_ops.h"

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

void Deposit(const char* clientsOpsPath, Client baseList[], int userID, string amount){
    int addDollars = 0, addCents = 0;
    StrToMoney(amount, addDollars, addCents);
    baseList[userID].balance.dollars += addDollars;
    baseList[userID].balance.cents += addCents;
    if(baseList[userID].balance.cents >= 100){
        baseList[userID].balance.dollars++;
        baseList[userID].balance.cents -= 100;
    }

    using std::to_string;
    string lineUpdate = to_string(baseList[userID].CI) + ',' + baseList[userID].name + ',' + 
                        to_string(baseList[userID].balance.dollars) + '.' + to_string(baseList[userID].balance.cents) + ",+" + 
                        to_string(addDollars) + '.' + to_string(addCents);
    ReplaceLine(clientsOpsPath, lineUpdate, userID + 1);
}