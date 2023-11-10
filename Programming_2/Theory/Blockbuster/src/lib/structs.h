#pragma once
#include <string>
using std::wstring;

struct Date{
    int year = 0, month = 0, day = 0;
};

enum MovStatus{ MOV_STATUS_RETURNED, MOV_STATUS_RENTED, MOV_STATUS_EXPIRED };

struct Movie{
    int ID = 0, duration = 0;
    wstring title;
    wstring genres[7] = {};     // A max of 6 genres can be stored here. The arr has 7 elements for boundary checks.
    wstring director;
    Date release;
    MovStatus status;
    wstring rentedTo = L"";
    Date rentedOn;
    Date expiry;
};

struct Frag{
    int ID = 0;
};

struct IntFrag : public Frag{
    int data;
};

struct WstrFrag : public Frag{
    wstring data;
};

struct User{
    int ID = 0;
    wstring name, movies;
};