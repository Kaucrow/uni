#pragma once
#include <string>
using std::wstring;

struct Movie{
    int ID = 0, duration = 0;
    wstring title;
    wstring genres[6] = {};
    wstring director;
    struct{
        int year = 0, month = 0, day = 0;
    } release;
};