#include <iostream>     // debug
#include <stdexcept>
#include "../misc/panic/panic.h"
#include "stack.h"

template <typename T>
Stack<T>::Stack()
    : size(0), top(nullptr)
{}

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

template Stack<int>::Stack();
template size_t Stack<int>::len();
template void Stack<int>::push(const int &data);
template int Stack<int>::pop();
template int& Stack<int>::peek();