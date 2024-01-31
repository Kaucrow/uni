#pragma once
#include <iostream>         // debug.

template <typename T>
struct List{
    T* data;
    int total, max;

    List(int initSize = 100) : data(new T[initSize * 2]), total(initSize), max(initSize * 2){}

    ~List(){
        delete[] data;
    }

    // Function to check and double the size of the array if necessary
    void CheckData(){
        if(total == max){
            max *= 2;
            T* temp = new T[max];
            for(int i = 0; i < total; i++)
                temp[i] = data[i];

            delete[] data;
            data = new T[max];

            for(int i = 0; i < total; i++)
                data[i] = temp[i];

            delete[] temp;
        }
    }
};