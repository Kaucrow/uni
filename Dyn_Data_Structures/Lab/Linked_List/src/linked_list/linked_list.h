#pragma once
#include <cstddef>
#include <string>

using std::string;

template <typename T>
struct Node {
    Node(T data) : data(data), next(nullptr) {}
    T data;
    Node* next; 
};

template <typename T>
using NodePtr = Node<T>*;

template <typename T>
class LinkedList {
    public:
        LinkedList();
        LinkedList(const LinkedList& other);
        LinkedList& operator=(const LinkedList& other);
        ~LinkedList();
        void append(T data);
        void clear();
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
        NodePtr<T> head;
        size_t size;
};

namespace LinkedListFn {
    LinkedList<string> from_row(string row);
}