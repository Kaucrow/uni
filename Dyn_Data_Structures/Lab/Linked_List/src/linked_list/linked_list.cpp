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
T& LinkedList<T>::operator[](const size_t idx) {
    NodePtr<T> current = this->head;
    size_t count = 0;

    while(current && count < idx) {
        current = current->next;
        count++;
    }

    if(!current) {
        throw std::out_of_range("[ ERR ] Linked list index out of bounds.");
    }

    return current->data;
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
    this->size++;
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

template <typename T>
size_t LinkedList<T>::len() {
    return this->size;
}

template <typename T>
void LinkedList<T>::sort(CompareFn<T> compare_fn) {
    this->head = this->merge_sort(this->head, compare_fn);
}

template <typename T>
NodePtr<T> LinkedList<T>::merge_sort(NodePtr<T> head, CompareFn<T> compare_fn) {
    if(!head || !head->next) {
        return head;
    }

    NodePtr<T> slow = head;
    NodePtr<T> fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    NodePtr<T> right = slow->next;
    slow->next = nullptr;

    head = merge_sort(head, compare_fn);
    right = merge_sort(right, compare_fn);

    return merge(head, right, compare_fn);
}

template <typename T>
NodePtr<T> LinkedList<T>::merge(NodePtr<T> left, NodePtr<T> right, CompareFn<T> compare_fn) {
    if(!left) {
        return right;
    }
    if(!right) {
        return left;
    }

    if(compare_fn(left->data, right->data)) {
        left->next = merge(left->next, right, compare_fn);
        return left;
    } else {
        right->next = merge(left, right->next, compare_fn);
        return right;
    }
}

namespace LinkedListFn {
    LinkedList<string> from_row(string row) {
        LinkedList<string> linked_list;
        size_t field_start = 0;
        size_t field_end = row.find(',');

        while(field_end != string::npos) {
            linked_list.append(row.substr(field_start, field_end - field_start));
            field_start = field_end + 1;
            field_end = row.find(',', field_start);
        }

        linked_list.append(row.substr(field_start, field_end - field_start));
        return linked_list;
    }
}

template LinkedList<Student>::LinkedList();
template LinkedList<Student>::~LinkedList();
template Student& LinkedList<Student>::operator[](size_t idx);
template void LinkedList<Student>::append(Student data);
template size_t LinkedList<Student>::len();
template void LinkedList<Student>::sort(CompareFn<Student> compare_fn);

template string& LinkedList<string>::operator[](size_t idx);
template LinkedList<string>::~LinkedList();
template void LinkedList<string>::sort(CompareFn<string> compare_fn);