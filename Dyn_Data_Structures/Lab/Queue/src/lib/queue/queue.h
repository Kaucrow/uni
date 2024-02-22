#pragma once
#include <cstddef>
#include <functional>
#include "../misc/panic/panic.h"
#include "../../../../Queue_Reqs/src/lib/request/request.h"

using std::function;

template <typename T>
struct Frame {
    Frame(const T &data) : data(data), next(nullptr) {}
    T data;
    Frame* next;
};

template <typename T>
using FramePtr = Frame<T>*;

template <typename T>
using CompareFn = function<bool(const T&, const T&)>;

template <typename T>
class Queue {
    public:
        Queue();
        ~Queue();
        size_t len();
        void push(const T &data);
        T pop();
        T& peek();
        void sort(CompareFn<T> compare_fn = std::less<T>());
    private:
        FramePtr<T> first;
        FramePtr<T> last;
        size_t size;
};

/*namespace QueueFn {
    Queue<Request> from_reqs_file(const char* filename, char delim);
}*/