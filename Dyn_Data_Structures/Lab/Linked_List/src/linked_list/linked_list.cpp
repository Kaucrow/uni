#pragma once
#include <iostream>     // debug
#include "../classes/student/student.h"
#include "linked_list.h"

template <typename T>
LinkedList<T>::LinkedList()
    : head(nullptr), size(0)
{}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) {
    copy_list(other);
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if(this != &other) {
        this->clear();
        this->copy_list(other);
    }
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    this->clear();    
}

template <typename T>
void LinkedList<T>::copy_list(const LinkedList<T>& other) {
    NodePtr<T> other_current = other.head;
    while(other_current) {
        this->append(other_current->data);
        other_current = other_current->next;
    }
}

template <typename T>
void LinkedList<T>::clear() {
    while(head) {
        NodePtr<T> temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
void LinkedList<T>::append(T data) {
    NodePtr<T> new_node = new Node<T>(data);

    if(!head) {
        head = new_node;
        return;
    }

    NodePtr<T> curr_node = this->head;

    while(curr_node->next) {
        curr_node = curr_node->next;
    }

    curr_node->next = new_node;
}

template LinkedList<Student>::LinkedList();
template LinkedList<Student>::~LinkedList();
template LinkedList<string>::~LinkedList();
template void LinkedList<Student>::append(Student data);

template Node<Student>::Node(Student data);

namespace LinkedListFn {
    LinkedList<string> from_row(string row) {
        LinkedList<string> linked_list;
        size_t field_start = 0;
        size_t field_end = row.find(',');

        while(field_end != string::npos) {
            linked_list.append(row.substr(field_start, field_end));
            field_start = field_end + 1;
            field_end = row.find(',', field_start);
        }

        linked_list.append(row.substr(field_start, field_end));

        return linked_list;
    }
}

