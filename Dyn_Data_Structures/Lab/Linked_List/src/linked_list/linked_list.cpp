#include <iostream>     // debug
#include <fstream>
#include <string>
#include "../classes/student/student.h"
#include "linked_list.h"
#include "../../../Stack/src/lib/misc/panic/panic.h"

using std::ifstream, std::ofstream, std::getline, std::string;

template <typename T>
LinkedList<T>::LinkedList()
    : head(nullptr), size(0)
{}

template <>
LinkedList<int>::LinkedList(string values) : head(nullptr), size(0) {
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
LinkedList<T>::LinkedList(const LinkedList& other) : head(nullptr), size(0) {
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
T LinkedList<T>::remove(const size_t idx) {
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

template <>
void LinkedList<Student>::obliterate_student(const size_t idx, const char* csv_path) {
    Student deleted = this->remove(idx);
    int csv_remove_line = deleted.csv_pos;

    string csv_path_str = string(csv_path);
    ifstream infile(csv_path_str);

    string tempfile_path = csv_path_str.substr(0, csv_path_str.rfind('/') + 1) + "temp.csv";
    ofstream tempfile(tempfile_path);

    string reading_line;
    while(getline(infile, reading_line)) {
        if(csv_remove_line == 1) {
            csv_remove_line--;
            continue;
        }
        tempfile << reading_line << '\n';
        csv_remove_line--;
    }

    infile.close();
    tempfile.close();
    
    std::remove(csv_path);
    std::rename(tempfile_path.c_str(), csv_path);
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
template Student LinkedList<Student>::remove(size_t idx);
template size_t LinkedList<Student>::len();
template void LinkedList<Student>::sort(CompareFn<Student> compare_fn);

template LinkedList<string>::LinkedList();
template LinkedList<string>::~LinkedList();
template string& LinkedList<string>::operator[](size_t idx);
template void LinkedList<string>::append(string data);
template string LinkedList<string>::remove(size_t idx);
template void LinkedList<string>::clear();
template void LinkedList<string>::sort(CompareFn<string> compare_fn);

template LinkedList<int>::LinkedList();
template LinkedList<int>::LinkedList(const LinkedList& other);
template LinkedList<int>::~LinkedList();
template int& LinkedList<int>::operator[](size_t idx);
template void LinkedList<int>::append(int data);
template int LinkedList<int>::remove(size_t idx);
template void LinkedList<int>::clear();
template void LinkedList<int>::sort(CompareFn<int> compare_fn);
template size_t LinkedList<int>::len();

template <typename T>
struct TreeNode {};

template <typename T>
using TreeNodePtr = TreeNode<T>*;

template LinkedList<TreeNodePtr<int>>::LinkedList();
template LinkedList<TreeNodePtr<int>>::~LinkedList();
template TreeNodePtr<int>& LinkedList<TreeNodePtr<int>>::operator[](size_t idx);
template void LinkedList<TreeNodePtr<int>>::append(TreeNodePtr<int> data);
template TreeNodePtr<int> LinkedList<TreeNodePtr<int>>::remove(size_t idx);
template size_t LinkedList<TreeNodePtr<int>>::len();
template LinkedList<int>& LinkedList<int>::operator=(const LinkedList& other);

template <typename T>
struct BSTNode {};

template <typename T>
using BSTNodePtr = BSTNode<T>*;

template LinkedList<BSTNodePtr<int>>::LinkedList();
template LinkedList<BSTNodePtr<int>>::~LinkedList();
template BSTNodePtr<int>& LinkedList<BSTNodePtr<int>>::operator[](size_t idx);
template void LinkedList<BSTNodePtr<int>>::append(BSTNodePtr<int> data);
template BSTNodePtr<int> LinkedList<BSTNodePtr<int>>::remove(size_t idx);
template size_t LinkedList<BSTNodePtr<int>>::len();