#pragma once
#include <cstddef>
#include <string>
#include <functional>
#include <iterator>
#include "../../../Stack/src/lib/misc/panic/panic.h"

using   std::ifstream, std::ofstream, std::getline, std::string,
        std::function;

// ==============
//  Declarations
// ==============

template <typename T>
struct Node {
    Node(T data);
    T data;
    Node* next;
};

template <typename T>
using NodePtr = Node<T>*;

template <typename T>
using CompareFn = function<bool(const T&, const T&)>;

template <typename T>
class LinkedList {
    public:
        LinkedList();
        LinkedList(string values);
        LinkedList(const LinkedList& other);
        LinkedList& operator=(const LinkedList& other);
        ~LinkedList();
        T& operator[](const size_t idx);
        void append(T data);
        int find(T data);
        T remove(const size_t idx);
        void obliterate_student(const size_t idx, const char* csv_path);
        void clear();
        void sort(CompareFn<T> compare_fn = std::less<T>());
        size_t len();
        class Iterator {
            public:
                Iterator(NodePtr<T> start);
                Iterator& operator++();
                bool operator!=(const Iterator& other) const;
                T& operator*() const;
            private:
                NodePtr<T> current;
        };
        Iterator begin() const;
        Iterator end() const;
    private:
        void copy_list(const LinkedList& other);
        NodePtr<T> merge_sort(NodePtr<T> head, CompareFn<T> compare_fn);
        NodePtr<T> merge(NodePtr<T> left, NodePtr<T> right, CompareFn<T> compare_fn);
        NodePtr<T> head;
        size_t size;
};

namespace LinkedListFn {
    LinkedList<string> from_row(string row);
};

// =============
//  Definitions
// =============

template <typename T>
inline Node<T>::Node(T data) : data(data), next(nullptr) {}

template <typename T>
inline LinkedList<T>::LinkedList() : head(nullptr), size(0) {}

template <>
inline LinkedList<int>::LinkedList(string values) : head(nullptr), size(0) {
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
inline LinkedList<T>::LinkedList(const LinkedList& other) : head(nullptr), size(0) {
    copy_list(other);
}

template <typename T>
inline LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if(this != &other) {
        this->clear();
        this->copy_list(other);
    }
    return *this;
}

template <typename T>
inline LinkedList<T>::~LinkedList() {
    this->clear();    
}

template <typename T>
inline T& LinkedList<T>::operator[](const size_t idx) {
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
inline void LinkedList<T>::copy_list(const LinkedList<T>& other) {
    NodePtr<T> other_current = other.head;
    while(other_current) {
        this->append(other_current->data);
        other_current = other_current->next;
    }
}

template <typename T>
inline void LinkedList<T>::clear() {
    while(head) {
        NodePtr<T> temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
inline int LinkedList<T>::find(T data) {
    NodePtr<T> curr = head;
    int idx = 0;
    while(curr) {
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
inline void LinkedList<T>::append(T data) {
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
inline T LinkedList<T>::remove(const size_t idx) {
    this->size--;
    NodePtr<T> previous = nullptr;
    NodePtr<T> current = this->head;
    size_t count = 0;

    while(current && count < idx) {
        previous = current;
        current = current->next;
        count++;
    }

    if(!current) {
        throw std::out_of_range("[ ERR ] Linked list index out of bounds.");
    }

    if(previous)
        previous->next = current->next;
    else
        this->head = current->next;

    T return_data = current->data;

    delete current;

    return return_data;
}

template <typename T>
inline size_t LinkedList<T>::len() {
    return this->size;
}

template <typename T>
inline void LinkedList<T>::sort(CompareFn<T> compare_fn) {
    this->head = this->merge_sort(this->head, compare_fn);
}

template <typename T>
inline NodePtr<T> LinkedList<T>::merge_sort(NodePtr<T> head, CompareFn<T> compare_fn) {
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
inline NodePtr<T> LinkedList<T>::merge(NodePtr<T> left, NodePtr<T> right, CompareFn<T> compare_fn) {
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
    inline LinkedList<string> from_row(string row) {
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

// ======================
//  Iterator definitions
// ======================

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