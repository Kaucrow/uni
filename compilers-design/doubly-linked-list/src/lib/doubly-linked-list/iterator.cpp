#include "doubly_linked_list.h"
#include "../student/student.h"

template <typename T>
DoublyLinkedList<T>::DoubleIterator::DoubleIterator(DoubleNodePtr<T> start) : current(start) {}

template <typename T>
typename DoublyLinkedList<T>::DoubleIterator& DoublyLinkedList<T>::DoubleIterator::operator++() {
    if(current) {
        current = current->next;
    }
    return *this;
}

template <typename T>
typename DoublyLinkedList<T>::DoubleIterator& DoublyLinkedList<T>::DoubleIterator::operator--() {
    if(current) {
        current = current->prev;
    }
    return *this;
}

template <typename T>
bool DoublyLinkedList<T>::DoubleIterator::operator!=(const DoublyLinkedList<T>::DoubleIterator& other) const {
    return current != other.current;
}

template <typename T>
T& DoublyLinkedList<T>::DoubleIterator::operator*() const {
    return current->data;
}

template <typename T>
typename DoublyLinkedList<T>::DoubleIterator DoublyLinkedList<T>::begin() const {
    return DoublyLinkedList<T>::DoubleIterator(this->head);
}

template <typename T>
typename DoublyLinkedList<T>::DoubleIterator DoublyLinkedList<T>::end() const {
    return DoublyLinkedList<T>::DoubleIterator(nullptr);
}

template <typename T>
typename DoublyLinkedList<T>::DoubleIterator DoublyLinkedList<T>::rbegin() const {
    return DoublyLinkedList<T>::DoubleIterator(this->tail);
}

template <typename T>
typename DoublyLinkedList<T>::DoubleIterator DoublyLinkedList<T>::rend() const {
    return DoublyLinkedList<T>::DoubleIterator(nullptr);
}

template DoublyLinkedList<string>::DoubleIterator& DoublyLinkedList<string>::DoubleIterator::operator++();
template string& DoublyLinkedList<string>::DoubleIterator::operator*() const;
template bool DoublyLinkedList<string>::DoubleIterator::operator!=(const DoublyLinkedList<string>::DoubleIterator& other) const;
template DoublyLinkedList<string>::DoubleIterator& DoublyLinkedList<string>::DoubleIterator::operator--();
template DoublyLinkedList<string>::DoubleIterator DoublyLinkedList<string>::begin() const;
template DoublyLinkedList<string>::DoubleIterator DoublyLinkedList<string>::end() const;
template DoublyLinkedList<string>::DoubleIterator DoublyLinkedList<string>::rbegin() const;
template DoublyLinkedList<string>::DoubleIterator DoublyLinkedList<string>::rend() const;

template DoublyLinkedList<Student>::DoubleIterator& DoublyLinkedList<Student>::DoubleIterator::operator++();
template DoublyLinkedList<Student>::DoubleIterator& DoublyLinkedList<Student>::DoubleIterator::operator--();
template Student& DoublyLinkedList<Student>::DoubleIterator::operator*() const;
template bool DoublyLinkedList<Student>::DoubleIterator::operator!=(const DoublyLinkedList<Student>::DoubleIterator& other) const;
template DoublyLinkedList<Student>::DoubleIterator DoublyLinkedList<Student>::begin() const;
template DoublyLinkedList<Student>::DoubleIterator DoublyLinkedList<Student>::end() const;
template DoublyLinkedList<Student>::DoubleIterator DoublyLinkedList<Student>::rbegin() const;
template DoublyLinkedList<Student>::DoubleIterator DoublyLinkedList<Student>::rend() const;