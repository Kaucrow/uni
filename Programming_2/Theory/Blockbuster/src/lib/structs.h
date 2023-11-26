#pragma once
#include <string>
#include "./basic_defs.h"

struct Date{
    int year = 0, month = 0, day = 0;
};

enum MovStatus{ MOV_STATUS_RETURNED, MOV_STATUS_RENTED, MOV_STATUS_EXPIRED };

struct Movie{
    int ID = 0, duration = 0;
    float price;
    pstring title;
    pstring genres[8] = {};     // A max of 7 genres can be stored here. The arr has 8 elements for boundary checks.
    pstring director;
    Date release;
    MovStatus status;
    pstring rentedTo;
    Date rentedOn;
    Date expiry;
};

struct Frag{
    int ID = 0;
};

struct IntFrag : public Frag{
    int data;
};

struct PStrFrag : public Frag{
    pstring data; 
};

struct User{
    int ID = 0;
    pstring name, movies;
};