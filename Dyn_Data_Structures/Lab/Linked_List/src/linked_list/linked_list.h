#pragma once
#include <cstddef>
#include <string>
#include <functional>
#include <iterator>

using std::string, std::function;

template <typename T>
struct Node {
    Node(T data) : data(data), next(nullptr) {}
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