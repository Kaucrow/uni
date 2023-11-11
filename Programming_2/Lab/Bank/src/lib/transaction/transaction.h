#pragma once
#include <iostream>     // for debug
#include <string>
#include "../structs.h"
using std::string;

void Deposit(const char* clientOpsPath, Client baseList[], int userID, string amount);

void Withdraw(const char* clientOpsPath, Client baseList[], int userID, string amount);

void Transfer(const char* clientOpsPath, Client baseList[], int userFromID, int userToID, string amount);