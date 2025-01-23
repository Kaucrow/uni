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
struct DoubleNode; // Forward declaration for DoubleNodePtr to work

template <typename T>
using DoubleNodePtr = DoubleNode<T>*; // Alias for DoubleNode<T>*

template <typename T>
struct DoubleNode {
    DoubleNode(T data, DoubleNodePtr<T> next, DoubleNodePtr<T> prev, size_t idx);
    T data;
    size_t idx;
    DoubleNode* next;
    DoubleNode* prev;
};

template <typename T>
using CompareFn = function<bool(const T&, const T&)>;

template <typename T>
class CircularDoublyLinkedList;

template <typename T>
class CircularDoublyLinkedList {
    public:
        CircularDoublyLinkedList();
        CircularDoublyLinkedList(string values);
        CircularDoublyLinkedList(const CircularDoublyLinkedList& other);
        CircularDoublyLinkedList& operator=(const CircularDoublyLinkedList& other);
        ~CircularDoublyLinkedList();
        DoubleNodePtr<T> operator[](const size_t idx);
        void append(T data);
        int find(T data);
        T remove(const size_t idx);
        void clear();
        void selection_sort(CompareFn<T> compare_fn = std::less<T>());
        void sort(CompareFn<T> compare_fn = std::less<T>());
        size_t len() const;
        bool is_last(DoubleNodePtr<T> node) const;
        bool is_first(DoubleNodePtr<T> node) const;
        class Iterator {
            public:
                Iterator(DoubleNodePtr<T> start, const CircularDoublyLinkedList<T>* list);
                Iterator& operator++();
                Iterator& operator--();
                bool operator!=(const Iterator& other) const;
                T& operator*() const;
            private:
                DoubleNodePtr<T> current;
                const CircularDoublyLinkedList<T>* parent_list;
        };
        Iterator begin() const;
        Iterator end() const;
        Iterator rbegin() const;
        Iterator rend() const;
    private:
        void copy_list(const CircularDoublyLinkedList& other);
        DoubleNodePtr<T> merge_sort(DoubleNodePtr<T> head, CompareFn<T> compare_fn);
        DoubleNodePtr<T> merge(DoubleNodePtr<T> left, DoubleNodePtr<T> right, CompareFn<T> compare_fn);
        DoubleNodePtr<T> head;
        DoubleNodePtr<T> tail;
        size_t size;
};

namespace CircularDoublyLinkedListFn {
    CircularDoublyLinkedList<string> from_row(string row);
};

// =============
//  Definitions
// =============

template <typename T>
inline DoubleNode<T>::DoubleNode(T data, DoubleNodePtr<T> next, DoubleNodePtr<T> prev, size_t idx) :
    data(data), next(next), prev(prev), idx(idx) {}

template <typename T>
inline CircularDoublyLinkedList<T>::CircularDoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

template <>
inline CircularDoublyLinkedList<int>::CircularDoublyLinkedList(string values) : head(nullptr), tail(nullptr), size(0) {
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
inline CircularDoublyLinkedList<T>::CircularDoublyLinkedList(const CircularDoublyLinkedList& other) : head(nullptr), size(0) {
    copy_list(other);
}

template <typename T>
inline CircularDoublyLinkedList<T>& CircularDoublyLinkedList<T>::operator=(const CircularDoublyLinkedList& other) {
    if (this != &other) {
        this->clear();
        this->copy_list(other);
    }
    return *this;
}

template <typename T>
inline CircularDoublyLinkedList<T>::~CircularDoublyLinkedList() {
    this->clear();    
}

template <typename T>
inline DoubleNodePtr<T> CircularDoublyLinkedList<T>::operator[](const size_t idx) {
    DoubleNodePtr<T> current = this->head;
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
inline void CircularDoublyLinkedList<T>::copy_list(const CircularDoublyLinkedList<T>& other) {
    DoubleNodePtr<T> other_current = other.head;
    while (other_current) {
        this->append(other_current->data);
        other_current = other_current->next;
    }
}

template <typename T>
inline void CircularDoublyLinkedList<T>::clear() {
    if (!head) return;

    DoubleNodePtr<T> curr = head;

    while (!this->is_last(curr)) {
        DoubleNodePtr<T> temp = curr;
        curr = curr->next;
        delete temp;
    }

    delete curr;

    this->head = nullptr;
    this->size = 0;
}

template <typename T>
inline int CircularDoublyLinkedList<T>::find(T data) {
    DoubleNodePtr<T> curr = head;
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
inline void CircularDoublyLinkedList<T>::append(T data) {
    DoubleNodePtr<T> new_node = new DoubleNode<T>(data, this->head, this->tail, this->size); 
    tail = new_node;

    if (!head) {
        head = new_node;
        head->prev = new_node;
        this->size++;
        return;
    }

    head->prev = new_node;

    DoubleNodePtr<T> curr_node = this->head;

    while (!this->is_last(curr_node)) {
        curr_node = curr_node->next;
    }
    
    curr_node->next = new_node;
    new_node->prev = curr_node;

    this->size++;
}

template <typename T>
inline T CircularDoublyLinkedList<T>::remove(const size_t idx) {
    DoubleNodePtr<T> previous = nullptr;
    DoubleNodePtr<T> current = this->head;
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
inline size_t CircularDoublyLinkedList<T>::len() const {
    return this->size;
}

template <typename T>
inline bool CircularDoublyLinkedList<T>::is_last(DoubleNodePtr<T> node) const {
    return node->idx == this->len() - 1;
}

template <typename T>
inline bool CircularDoublyLinkedList<T>::is_first(DoubleNodePtr<T> node) const {
    return node->idx == 0;
}

namespace CircularDoublyLinkedListFn {
    inline CircularDoublyLinkedList<string> from_row(string row) {
        CircularDoublyLinkedList<string> linked_list;
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
CircularDoublyLinkedList<T>::Iterator::Iterator(DoubleNodePtr<T> start, const CircularDoublyLinkedList<T>* list) : current(start), parent_list(list) {}

template <typename T>
typename CircularDoublyLinkedList<T>::Iterator& CircularDoublyLinkedList<T>::Iterator::operator++() {
    if (parent_list->is_last(current)) {
        current = nullptr;
    } else if (current) {
        current = current->next;
    }
    return *this;
}

template <typename T>
typename CircularDoublyLinkedList<T>::Iterator& CircularDoublyLinkedList<T>::Iterator::operator--() {
    if (parent_list->is_first(current)) {
        current = nullptr;
    } else if (current) {
        current = current->prev;
    }
    return *this;
}

template <typename T>
bool CircularDoublyLinkedList<T>::Iterator::operator!=(const CircularDoublyLinkedList<T>::Iterator& other) const {
    return current != other.current;
}

template <typename T>
T& CircularDoublyLinkedList<T>::Iterator::operator*() const {
    return current->data;
}

template <typename T>
typename CircularDoublyLinkedList<T>::Iterator CircularDoublyLinkedList<T>::begin() const {
    return CircularDoublyLinkedList<T>::Iterator(this->head, this);
}

template <typename T>
typename CircularDoublyLinkedList<T>::Iterator CircularDoublyLinkedList<T>::end() const {
    return CircularDoublyLinkedList<T>::Iterator(nullptr, this);
}

template <typename T>
typename CircularDoublyLinkedList<T>::Iterator CircularDoublyLinkedList<T>::rbegin() const {
    return CircularDoublyLinkedList<T>::Iterator(this->tail, this);
}

template <typename T>
typename CircularDoublyLinkedList<T>::Iterator CircularDoublyLinkedList<T>::rend() const {
    return CircularDoublyLinkedList<T>::Iterator(nullptr, this);
}