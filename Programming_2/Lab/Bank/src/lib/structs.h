#pragma once
#include <string>
using std::string;

enum MovStatus{ MOV_STATUS_RETURNED, MOV_STATUS_RENTED, MOV_STATUS_EXPIRED };

struct Client{
    int ID = 0, CI = 0;
    struct{
        int dollars = 0, cents = 0;
    } balance;
    long long int accNum;
    string name, accType;
    bool suspend;
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