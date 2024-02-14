#pragma once
#include <cstddef>
#include <string>

using std::string;

template <typename T>
struct Frame {
    Frame(const T &data) : data(data), next(nullptr) {}
    T data;
    Frame* next;
};

template <typename T>
using FramePtr = Frame<T>*;

template <typename T>
class Stack {
    public:
        Stack();
        ~Stack();
        size_t len();
        void push(const T &data);
        T pop();
        T& peek();
        string to_string();
    private:
        FramePtr<T> top;
        size_t size;
};