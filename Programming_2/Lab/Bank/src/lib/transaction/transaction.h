#pragma once
#include <iostream>     // for debug
#include <string>
#include "../structs.h"
using std::string;

void Deposit(const char* clientsOpsPath, Client baseList[], int userID, string amount);

void Withdraw(const char* clientsOpsPath, Client baseList[], int userID, string amount);