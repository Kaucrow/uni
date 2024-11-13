#pragma once
#include <string>
using std::string;

enum AccType{ ACC_DEBIT = 1, ACC_CURRENT = 2};

struct Client{
    int ID = 0, CI = 0;
    struct{
        int dollars = 0, cents = 0;
    } balance;
    long long int accNum;
    AccType accType;
    string name;
    bool suspended;
};

struct Frag{
    int ID = 0;
};

struct IntFrag : public Frag{
    int data;
};

struct LLIntFrag : public Frag{
    long long int data;
};

struct StrFrag : public Frag{
    string data;
};