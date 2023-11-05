#pragma once
#include <string>
using std::wstring;

struct Date{
    int year = 0, month = 0, day = 0;
};

struct Movie{
    int ID = 0, duration = 0;
    wstring title;
    wstring genres[6] = {};
    wstring director;
    Date release;
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