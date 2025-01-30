#include <fstream>
#include <string>
#include "queue.h"

using std::string, std::ifstream;

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

    if(!first)
        last = nullptr;

    return ret;
}

template <typename T>
T& Queue<T>::peek() {
    if(!this->first)
        panic("[ ERR ] Tried to perform a peek() on an empty queue.");

    return first->data;
}

template <typename T>
void Queue<T>::sort(CompareFn<T> compare_fn) {
    if(!first) return;

    FramePtr<T> first_check = first;

    FramePtr<T> to_relocate;
    FramePtr<T> to_relocate_prev;

    FramePtr<T> first_relocate = nullptr;

    while(first_check->next) {
        to_relocate = first_check;
        to_relocate_prev = first_check;
        FramePtr<T> previous = first_check;
        FramePtr<T> checking = first_check->next;

        while(checking) {
            if(compare_fn(checking->data, to_relocate->data)) {
                to_relocate = checking;
                to_relocate_prev = previous;
            }
            checking = checking->next;
            previous = previous->next;
        }

        if(to_relocate == first_check)
            first_check = first_check->next;

        to_relocate_prev->next = to_relocate->next;
        to_relocate->next = first;
        first = to_relocate;

        if(!first_relocate)
            first_relocate = to_relocate;
    }
    first_relocate->next = nullptr;
    last = first_relocate;
    first_check->next = first;
    first = first_check;
}

namespace QueueFn {
    Queue<Request> from_reqs_file(const char* filename, char delim) {
        Queue<Request> queue;

        ifstream reqs_file(filename);

        if(!reqs_file)
            panic("[ ERR ] Could not open file on `from_reqs_file()` fn");

        string reading_line;

        getline(reqs_file, reading_line);

        while(getline(reqs_file, reading_line))
            queue.push(Request(reading_line, delim));

        return queue;
    }
}

template Queue<int>::Queue();
template Queue<int>::~Queue();
template size_t Queue<int>::len();
template void Queue<int>::push(const int &data);
template int Queue<int>::pop();
template int& Queue<int>::peek();
template void Queue<int>::sort(CompareFn<int> compare_fn);

template Queue<Request>::Queue();
template Queue<Request>::~Queue();
template size_t Queue<Request>::len();
template void Queue<Request>::push(const Request &data);
template Request Queue<Request>::pop();
template Request& Queue<Request>::peek();
template void Queue<Request>::sort(CompareFn<Request> compare_fn);