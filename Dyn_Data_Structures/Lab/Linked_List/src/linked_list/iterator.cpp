#include "linked_list.h"
#include "../classes/student/student.h"

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

template LinkedList<int>::Iterator& LinkedList<int>::Iterator::operator++();
template int& LinkedList<int>::Iterator::operator*() const;
template bool LinkedList<int>::Iterator::operator!=(const LinkedList<int>::Iterator& other) const;
template LinkedList<int>::Iterator LinkedList<int>::begin() const;
template LinkedList<int>::Iterator LinkedList<int>::end() const;

template LinkedList<Student>::Iterator& LinkedList<Student>::Iterator::operator++();
template Student& LinkedList<Student>::Iterator::operator*() const;
template bool LinkedList<Student>::Iterator::operator!=(const LinkedList<Student>::Iterator& other) const;
template LinkedList<Student>::Iterator LinkedList<Student>::begin() const;
template LinkedList<Student>::Iterator LinkedList<Student>::end() const;

template <typename T>
struct TreeNode {};

template <typename T>
using TreeNodePtr = TreeNode<T>*;

template LinkedList<TreeNodePtr<int>>::Iterator& LinkedList<TreeNodePtr<int>>::Iterator::operator++();
template TreeNodePtr<int>& LinkedList<TreeNodePtr<int>>::Iterator::operator*() const;
template bool LinkedList<TreeNodePtr<int>>::Iterator::operator!=(const LinkedList<TreeNodePtr<int>>::Iterator& other) const;
template LinkedList<TreeNodePtr<int>>::Iterator LinkedList<TreeNodePtr<int>>::begin() const;
template LinkedList<TreeNodePtr<int>>::Iterator LinkedList<TreeNodePtr<int>>::end() const;