#include <iostream>     // debug
#include <stdexcept>
#include <algorithm>
#include "../misc/panic/panic.h"
#include "stack.h"

template <typename T>
Stack<T>::Stack()
    : size(0), top(nullptr)
{}

template <typename T>
Stack<T>::~Stack() {
    while(top) {
        this->pop();
    }
}

template <typename T>
size_t Stack<T>::len() {
    return this->size;
}

template <typename T>
void Stack<T>::push(const T &data) {
    this->size++;

    FramePtr<T> new_frame = new Frame(data);

    new_frame->next = top;
    
    top = new_frame;
}

template <typename T>
T Stack<T>::pop() {
    this->size--;

    if(!this->top)
        panic("[ ERR ] Tried to perform a pop() on an empty stack.");

    T ret = top->data;
    FramePtr<T> next = top->next;
    delete top;
    top = next;
    return ret;
}

template <typename T>
T& Stack<T>::peek() {
    if(!this->top)
        panic("[ ERR ] Tried to perform a peek() on an empty stack.");

    return top->data;
}

template <typename T>
string Stack<T>::to_string() {
    FramePtr<T> frames[size];
    FramePtr<T> next = top;

    for(int i = size - 1; i >= 0; i--) {
        frames[i] = next;
        next = next->next;
    }

    string ret = "[";

    for(int i = 0; i < size; i++)
        ret.append(std::to_string(frames[i]->data) + ", ");

    ret = ret.substr(0, ret.size() - 2);
    ret.append("]");

    return ret;
}

template Stack<int>::Stack();
template Stack<int>::~Stack();
template size_t Stack<int>::len();
template void Stack<int>::push(const int &data);
template int Stack<int>::pop();
template int& Stack<int>::peek();
template string Stack<int>::to_string();