#pragma once
#include <cstddef>
#include <string>
#include <functional>
#include <iterator>
#include <iostream>
#include "../../../linked-list/src/linked-list/linked_list.h"

using   std::string, std::function, std::ifstream, std::ofstream,
        std::getline;

// ==============
//  Declarations
// ==============

template <typename T>
struct DoubleNode {
    DoubleNode(T data) : data(data), next(nullptr), prev(nullptr) {}
    DoubleNode(Node<T> other) : data(other->data), next(other->next), prev(nullptr) {}
    T data;
    DoubleNode* next;
    DoubleNode* prev;
};

template <typename T>
using DoubleNodePtr = DoubleNode<T>*;

template <typename T>
using CompareFn = function<bool(const T&, const T&)>;

template <typename T>
class DoublyLinkedList {
    public:
        DoublyLinkedList();
        DoublyLinkedList(const DoublyLinkedList& other);
        DoublyLinkedList(const LinkedList<T>& other);
        DoublyLinkedList& operator=(const DoublyLinkedList& other);
        ~DoublyLinkedList();
        T& operator[](const size_t idx);
        void append(T data);
        void insert(T data, const size_t idx);
        T remove(const size_t idx);
        void clear();
        void sort(CompareFn<T> compare_fn = std::less<T>());
        size_t len();
        class DoubleIterator {
            public:
                DoubleIterator(DoubleNodePtr<T> start);
                DoubleIterator& operator++();
                DoubleIterator& operator--();
                bool operator!=(const DoubleIterator& other) const;
                T& operator*() const;
            private:
                DoubleNodePtr<T> current;
        };
        DoubleIterator begin() const;
        DoubleIterator end() const;
        DoubleIterator rbegin() const;
        DoubleIterator rend() const;
    private:
        DoubleNodePtr<T> get_node(size_t idx);
        void copy_list(const DoublyLinkedList& other);
        void copy_list(const LinkedList<T>& other);
        DoubleNodePtr<T> merge_sort(DoubleNodePtr<T> head, CompareFn<T> compare_fn);
        DoubleNodePtr<T> merge(DoubleNodePtr<T> left, DoubleNodePtr<T> right, CompareFn<T> compare_fn);
        DoubleNodePtr<T> split(DoubleNodePtr<T> head);
        DoubleNodePtr<T> head;
        DoubleNodePtr<T> tail;
        size_t size;
};

namespace DoublyLinkedListFn {
    DoublyLinkedList<string> from_row(string row);
};

// =============
//  Definitions
// =============

template <typename T>
inline DoublyLinkedList<T>::DoublyLinkedList()
    : head(nullptr), tail(nullptr), size(0)
{}

template <typename T>
inline DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other)
    : head(nullptr), tail(nullptr), size(0)
{
    copy_list(other);
}

template <typename T>
inline DoublyLinkedList<T>::DoublyLinkedList(const LinkedList<T>& other)
    : head(nullptr), tail(nullptr), size(0)
{
    copy_list(other);
}

template <typename T>
inline DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other) {
    if(this != &other) {
        this->clear();
        this->copy_list(other);
    }
    return *this;
}

template <typename T>
inline DoublyLinkedList<T>::~DoublyLinkedList() {
    this->clear();    
}

template <typename T>
inline T& DoublyLinkedList<T>::operator[](const size_t idx) {
    DoubleNodePtr<T> current = this->head;
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
inline DoubleNodePtr<T> DoublyLinkedList<T>::get_node(size_t idx) {
    DoubleNodePtr<T> current;
    //size_t count = 0;

    if(idx < size / 2) {
        current = this->head;
        while(current && idx) {
            current = current->next;
            idx--;
            //count++;
        }
    } else {
        current = this->tail;
        while(current && idx) {
            current = current->prev;
            idx--;
            //count--;
        }
    }

    if(!current) {
        throw std::out_of_range("[ ERR ] Linked list index out of bounds.");
    }

    return current;
}

template <typename T>
inline void DoublyLinkedList<T>::copy_list(const DoublyLinkedList<T>& other) {
    DoubleNodePtr<T> other_current = other.head;
    while(other_current) {
        this->append(other_current->data);
        other_current = other_current->next;
    }
}

template <typename T>
inline void DoublyLinkedList<T>::copy_list(const LinkedList<T>& other) {
    NodePtr<T> other_current = other.head;
    while(other_current) {
        this->append(other_current->data);
        other_current = other_current->next;
    }
}

template <typename T>
inline void DoublyLinkedList<T>::clear() {
    while(head) {
        DoubleNodePtr<T> temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
inline void DoublyLinkedList<T>::append(T data) {
    this->size++;
    DoubleNodePtr<T> new_node = new DoubleNode<T>(data);

    if(!tail) {
        tail = new_node;
        head = new_node;
    } else {
        new_node->prev = tail;
        tail->next = new_node;
        tail = new_node;
    }
}

template <typename T>
inline void DoublyLinkedList<T>::insert(T data, const size_t idx) {
    this->size++;
    DoubleNodePtr<T> new_node = new DoubleNode<T>(data);
    DoubleNodePtr<T> replace = get_node(idx);
    if(replace->prev) {
        replace->prev->next = new_node;
        new_node->prev = replace->prev;
    } else {
        head = new_node;
    }
    new_node->next = replace;
    replace->prev = new_node;
    std::cout << "REPLACE: " << replace->data << '\n';
}

template <typename T>
inline T DoublyLinkedList<T>::remove(const size_t idx) {
    this->size--;
    DoubleNodePtr<T> previous = nullptr;
    DoubleNodePtr<T> current = get_node(idx);

    if(current->prev) {
        current->prev->next = current->next;
    }

    if(current->next) {
        current->next->prev = current->prev;
    }

    T return_data = current->data;

    delete current;

    return return_data;
}

template <typename T>
inline size_t DoublyLinkedList<T>::len() {
    return this->size;
}

/*
template <typename T>
inline void DoublyLinkedList<T>::sort(CompareFn<T> compare_fn) {
    this->head = this->merge_sort(this->head, compare_fn);
}

template <typename T>
inline DoubleNodePtr<T> DoublyLinkedList<T>::split(DoubleNodePtr<T> head) {
    DoubleNodePtr<T> fast = head;
    DoubleNodePtr<T> slow = head;

    // Move fast pointer two steps and slow pointer
    // one step until fast reaches the end
    while (fast && fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    // Split the list into two halves
    DoubleNodePtr<T> temp = slow->next;
    slow->next = nullptr;
    if (temp) {
        temp->prev = nullptr;
    }
    return temp;
}

template <typename T>
inline DoubleNodePtr<T> DoublyLinkedList<T>::merge_sort(DoubleNodePtr<T> head, CompareFn<T> compare_fn) {
    // Base case: if the list is empty or has only one node, 
    // it's already sorted
    if(!head || !head->next) {
        return head;
    }

    DoubleNodePtr<T> right = split(head);

    this->head = merge_sort(this->head, compare_fn);
    right = merge_sort(right, compare_fn);

    return merge(head, right, compare_fn);
}

template <typename T>
inline DoubleNodePtr<T> DoublyLinkedList<T>::merge(DoubleNodePtr<T> left, DoubleNodePtr<T> right, CompareFn<T> compare_fn) {
    // If one of the lists is empty, return the other list
    if (!left) {
        return right;
    }
    if (!right) {
        return left;
    }

    // Choose the node with the smallest value based on the comparison function
    if (compare_fn(left->data, right->data)) {
        left->next = merge(left->next, right, compare_fn);
        if (left->next) {
            left->next->prev = left;  // Update the prev pointer of the next node
        }
        left->prev = nullptr;
        return left;
    } else {
        right->next = merge(left, right->next, compare_fn);
        if (right->next) {
            right->next->prev = right;  // Update the prev pointer of the next node
        }
        right->prev = nullptr;
        return right;
    }
}
*/

template <typename T>
inline void DoublyLinkedList<T>::sort(CompareFn<T> compare_fn) {
    // Sort the list starting from the head
    this->head = this->merge_sort(this->head, compare_fn);
    // Update the tail after sorting
    DoubleNodePtr<T> temp = this->head;
    while (temp && temp->next) {
        temp = temp->next;
    }
    this->tail = temp;
}

template <typename T>
inline DoubleNodePtr<T> DoublyLinkedList<T>::split(DoubleNodePtr<T> head) {
    DoubleNodePtr<T> fast = head;
    DoubleNodePtr<T> slow = head;

    // Move fast pointer two steps and slow pointer one step until fast reaches the end
    while (fast && fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    // Split the list into two halves
    DoubleNodePtr<T> temp = slow->next;
    slow->next = nullptr;
    if (temp) {
        temp->prev = nullptr;
    }
    return temp;
}

template <typename T>
inline DoubleNodePtr<T> DoublyLinkedList<T>::merge_sort(DoubleNodePtr<T> head, CompareFn<T> compare_fn) {
    // Base case: if the list is empty or has only one node, it's already sorted
    if (!head || !head->next) {
        return head;
    }

    // Split the list into two halves
    DoubleNodePtr<T> right = split(head);

    // Recursively sort both halves
    DoubleNodePtr<T> left = merge_sort(head, compare_fn);
    right = merge_sort(right, compare_fn);

    // Merge the sorted halves
    return merge(left, right, compare_fn);
}

template <typename T>
inline DoubleNodePtr<T> DoublyLinkedList<T>::merge(DoubleNodePtr<T> left, DoubleNodePtr<T> right, CompareFn<T> compare_fn) {
    // If one of the lists is empty, return the other list
    if (!left) {
        return right;
    }
    if (!right) {
        return left;
    }

    // Choose the node with the smallest value based on the comparison function
    if (compare_fn(left->data, right->data)) {
        left->next = merge(left->next, right, compare_fn);
        if (left->next) {
            left->next->prev = left;  // Update the prev pointer of the next node
        }
        left->prev = nullptr;
        return left;
    } else {
        right->next = merge(left, right->next, compare_fn);
        if (right->next) {
            right->next->prev = right;  // Update the prev pointer of the next node
        }
        right->prev = nullptr;
        return right;
    }
}


namespace DoublyLinkedListFn {
    inline DoublyLinkedList<string> from_row(string row) {
        DoublyLinkedList<string> linked_list;
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
inline DoublyLinkedList<T>::DoubleIterator::DoubleIterator(DoubleNodePtr<T> start) : current(start) {}

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