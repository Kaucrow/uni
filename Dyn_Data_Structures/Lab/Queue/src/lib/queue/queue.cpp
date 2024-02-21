#include "queue.h"

template <typename T>
Queue<T>::Queue()
    : first(nullptr), last(nullptr), size(0)
{}

template <typename T>
Queue<T>::~Queue() {
    while(first) {
        this->pop();
    }
}

template <typename T>
size_t Queue<T>::len() {
    return this->size;
}

template <typename T>
void Queue<T>::push(const T &data) {
    this->size++;

    FramePtr<T> new_frame = new Frame(data);

    if(!first) {
        first = new_frame;
        last = new_frame;
    } else {
        last->next = new_frame;
        last = new_frame;
    }
}

template <typename T>
T Queue<T>::pop() {
    this->size--;

    if(!this->first)
        panic("[ ERR ] Tried to perform a pop() on an empty queue.");

    T ret = first->data;
    FramePtr<T> next = first->next;
    delete first;
    first = next;
    return ret;
}

template <typename T>
T& Queue<T>::peek() {
    if(!this->first)
        panic("[ ERR ] Tried to perform a peek() on an empty queue.");

    return first->data;
}

template Queue<int>::Queue();
template Queue<int>::~Queue();
template size_t Queue<int>::len();
template void Queue<int>::push(const int &data);
template int Queue<int>::pop();
template int& Queue<int>::peek();