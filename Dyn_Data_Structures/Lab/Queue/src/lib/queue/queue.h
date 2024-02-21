#pragma once
#include <cstddef>
#include <string>
#include "../misc/panic/panic.h"

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
class Queue {
    public:
        Queue();
        ~Queue();
        size_t len();
        void push(const T &data);
        T pop();
        T& peek();
    private:
        FramePtr<T> first;
        FramePtr<T> last;
        size_t size;
};