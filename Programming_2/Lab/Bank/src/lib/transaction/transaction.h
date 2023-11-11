#pragma once
#include <iostream>     // for debug
#include <string>
#include "../structs.h"

using std::string;

/**
 * All functions defined here share 3 params:
 * @param clientOpsPath - Path to the client_ops.csv file.
 * @param baseList - Base client list.
 * @param amount - string object containing the mount of money to deposit/withdraw/transfer.
*/

/**
 * @brief Performs a money deposit to a client account.
 * @param clientID - ID of the client to deposit money to.
*/
void Deposit(const char* clientOpsPath, Client baseList[], int clientID, string amount);

/**
 * @brief Performs a money withdrawal to a client account.
 * @param clientID - ID of the client to withdraw money from.
*/
void Withdraw(const char* clientOpsPath, Client baseList[], int clientID, string amount);

/**
 * @brief Performs a money transfer from one client account to another.
 * @param clientFromID - ID of the client transferring the money.
 * @param clientToID - ID of the beneficiary client.
*/
void Transfer(const char* clientOpsPath, Client baseList[], int clientFromID, int clientToID, string amount);