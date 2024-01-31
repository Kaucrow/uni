#include "linked_list.h"

template <typename T>
LinkedList<T>::Iterator::Iterator(NodePtr<T> start) : current(start) {}

template <typename T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++() {
    if(current) {
        current = current->next;
    }
    return *this;
}

template <typename T>
bool LinkedList<T>::Iterator::operator!=(const LinkedList<T>::Iterator& other) const {
    return current != other.current;
}

template <typename T>
T& LinkedList<T>::Iterator::operator*() const {
    return current->data;
}

template <typename T>
typename LinkedList<T>::Iterator LinkedList<T>::begin() const {
    return LinkedList<T>::Iterator(this->head);
}

template <typename T>
typename LinkedList<T>::Iterator LinkedList<T>::end() const {
    return LinkedList<T>::Iterator(nullptr);
}

template LinkedList<string>::Iterator& LinkedList<string>::Iterator::operator++();
template string& LinkedList<string>::Iterator::operator*() const;
template bool LinkedList<string>::Iterator::operator!=(const LinkedList<string>::Iterator& other) const;
template LinkedList<string>::Iterator LinkedList<string>::begin() const;
template LinkedList<string>::Iterator LinkedList<string>::end() const;