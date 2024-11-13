#pragma once
#include <cstddef>
#include <string>
#include <functional>
#include <iterator>
#include <iostream>
#include "../linked_list/linked_list.h"

using std::string, std::function;

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
        void obliterate_student(const size_t idx, const char* csv_path);
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
        DoubleNodePtr<T> head;
        DoubleNodePtr<T> tail;
        size_t size;
};

namespace DoublyLinkedListFn {
    DoublyLinkedList<string> from_row(string row);
};