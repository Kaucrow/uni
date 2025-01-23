#pragma once
#include <iostream>     // debug
#include <cstddef>
#include <string>
#include <functional>
#include <iterator>
#include "../../../Stack/src/lib/misc/panic/panic.h"

using   std::ifstream, std::ofstream, std::getline, std::string,
        std::function, std::cout;

// ==============
//  Declarations
// ==============

template <typename T>
struct Node; // Forward declaration for NodePtr to work

template <typename T>
using NodePtr = Node<T>*; // Alias for Node<T>*

template <typename T>
struct Node {
    Node(T data, NodePtr<T> next, size_t idx);
    T data;
    size_t idx;
    Node* next;
};

template <typename T>
using CompareFn = function<bool(const T&, const T&)>;

template <typename T>
class CircularLinkedList;

template <typename T>
class CircularLinkedList {
    public:
        CircularLinkedList();
        CircularLinkedList(string values);
        CircularLinkedList(const CircularLinkedList& other);
        CircularLinkedList& operator=(const CircularLinkedList& other);
        ~CircularLinkedList();
        NodePtr<T> operator[](const size_t idx);
        void append(T data);
        int find(T data);
        T remove(const size_t idx);
        void clear();
        void selection_sort(CompareFn<T> compare_fn = std::less<T>());
        void sort(CompareFn<T> compare_fn = std::less<T>());
        size_t len() const;
        bool is_last(NodePtr<T> node) const;
        class Iterator {
            public:
                Iterator(NodePtr<T> start, const CircularLinkedList<T>* list);
                Iterator& operator++();
                bool operator!=(const Iterator& other) const;
                T& operator*() const;
            private:
                NodePtr<T> current;
                const CircularLinkedList<T>* parent_list;
        };
        Iterator begin() const;
        Iterator end() const;
    private:
        void copy_list(const CircularLinkedList& other);
        NodePtr<T> merge_sort(NodePtr<T> head, CompareFn<T> compare_fn);
        NodePtr<T> merge(NodePtr<T> left, NodePtr<T> right, CompareFn<T> compare_fn);
        NodePtr<T> head;
        size_t size;
};

namespace CircularLinkedListFn {
    CircularLinkedList<string> from_row(string row);
};

// =============
//  Definitions
// =============

template <typename T>
inline Node<T>::Node(T data, NodePtr<T> next, size_t idx) : data(data), next(next), idx(idx) {}

template <typename T>
inline CircularLinkedList<T>::CircularLinkedList() : head(nullptr), size(0) {}

template <>
inline CircularLinkedList<int>::CircularLinkedList(string values) : head(nullptr), size(0) {
    if (values.empty()) return;

    size_t next_comma = string::npos;

    try {
        while (next_comma = values.find(',') != string::npos) {
            this->append(stoi(values.substr(0, next_comma)));

            values = values.substr(next_comma + 1);
        }

        this->append(stoi(values));
    } catch (...) {
        panic("Could not parse values string `" + values + "`. Remember to use commas as separators.");
    }
}

template <typename T>
inline CircularLinkedList<T>::CircularLinkedList(const CircularLinkedList& other) : head(nullptr), size(0) {
    copy_list(other);
}

template <typename T>
inline CircularLinkedList<T>& CircularLinkedList<T>::operator=(const CircularLinkedList& other) {
    if (this != &other) {
        this->clear();
        this->copy_list(other);
    }
    return *this;
}

template <typename T>
inline CircularLinkedList<T>::~CircularLinkedList() {
    this->clear();    
}

template <typename T>
inline NodePtr<T> CircularLinkedList<T>::operator[](const size_t idx) {
    NodePtr<T> current = this->head;
    size_t count = 0;

    while (!is_last(current) && count < idx) {
        current = current->next;
        count++;
    }

    if (!current) {
        throw std::out_of_range("[ ERR ] Linked list index out of bounds.");
    }

    return current;
}

template <typename T>
inline void CircularLinkedList<T>::copy_list(const CircularLinkedList<T>& other) {
    NodePtr<T> other_current = other.head;
    while (other_current) {
        this->append(other_current->data);
        other_current = other_current->next;
    }
}

template <typename T>
inline void CircularLinkedList<T>::clear() {
    if (!head) return;

    NodePtr<T> curr = head;

    while (!this->is_last(curr)) {
        NodePtr<T> temp = curr;
        curr = curr->next;
        delete temp;
    }

    delete curr;

    this->head = nullptr;
    this->size = 0;
}

template <typename T>
inline int CircularLinkedList<T>::find(T data) {
    NodePtr<T> curr = head;
    int idx = 0;
    while (!is_last(curr)) {
        if (curr->data == data)
            return idx;
        else {
            idx++;
            curr = curr->next;
        }
    }
    return -1;
}

template <typename T>
inline void CircularLinkedList<T>::append(T data) {
    NodePtr<T> new_node = new Node<T>(data, this->head, this->size);
    
    if (!head) {
        head = new_node;
        this->size++;
        return;
    }

    NodePtr<T> curr_node = this->head;

    while (!this->is_last(curr_node)) {
        curr_node = curr_node->next;
    }
    
    curr_node->next = new_node;

    this->size++;
}

template <typename T>
inline T CircularLinkedList<T>::remove(const size_t idx) {
    NodePtr<T> previous = nullptr;
    NodePtr<T> current = this->head;
    size_t count = 0;

    while (!is_last(current) && count < idx) {
        previous = current;
        current = current->next;
        count++;
    }

    if (!current) {
        throw std::out_of_range("[ ERR ] Linked list index out of bounds.");
    }

    if (previous)
        previous->next = current->next;
    else
        this->head = current->next;

    T return_data = current->data;

    delete current;

    this->size--;

    return return_data;
}

template <typename T>
inline size_t CircularLinkedList<T>::len() const {
    return this->size;
}

template <typename T>
inline bool CircularLinkedList<T>::is_last(NodePtr<T> node) const {
    return node->idx == this->len() - 1;
}

namespace CircularLinkedListFn {
    inline CircularLinkedList<string> from_row(string row) {
        CircularLinkedList<string> linked_list;
        size_t field_start = 0;
        size_t field_end = row.find(',');

        while (field_end != string::npos) {
            linked_list.append(row.substr(field_start, field_end - field_start));
            field_start = field_end + 1;
            field_end = row.find(',', field_start);
        }

        linked_list.append(row.substr(field_start, field_end - field_start));
        return linked_list;
    }
}

// ======================
//  Iterator definitions
// ======================

template <typename T>
CircularLinkedList<T>::Iterator::Iterator(NodePtr<T> start, const CircularLinkedList<T>* list) : current(start), parent_list(list) {}

template <typename T>
typename CircularLinkedList<T>::Iterator& CircularLinkedList<T>::Iterator::operator++() {
    if (parent_list->is_last(current)) {
        current = nullptr;
    } else if (current) {
        current = current->next;
    }
    return *this;
}

template <typename T>
bool CircularLinkedList<T>::Iterator::operator!=(const CircularLinkedList<T>::Iterator& other) const {
    return current != other.current;
}

template <typename T>
T& CircularLinkedList<T>::Iterator::operator*() const {
    return current->data;
}

template <typename T>
typename CircularLinkedList<T>::Iterator CircularLinkedList<T>::begin() const {
    return CircularLinkedList<T>::Iterator(this->head, this);
}

template <typename T>
typename CircularLinkedList<T>::Iterator CircularLinkedList<T>::end() const {
    return CircularLinkedList<T>::Iterator(nullptr, this);
}