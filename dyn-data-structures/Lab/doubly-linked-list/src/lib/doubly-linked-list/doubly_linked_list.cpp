#include <iostream>     // debug
#include <fstream>
#include <string>
#include "../student/student.h"
#include "../linked-list/linked_list.h"
#include "doubly_linked_list.h"

using std::ifstream, std::ofstream, std::getline, std::string;

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList()
    : head(nullptr), tail(nullptr), size(0)
{}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other)
    : head(nullptr), tail(nullptr), size(0)
{
    copy_list(other);
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const LinkedList<T>& other)
    : head(nullptr), tail(nullptr), size(0)
{
    copy_list(other);
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other) {
    if(this != &other) {
        this->clear();
        this->copy_list(other);
    }
    return *this;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    this->clear();    
}

template <typename T>
T& DoublyLinkedList<T>::operator[](const size_t idx) {
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
DoubleNodePtr<T> DoublyLinkedList<T>::get_node(size_t idx) {
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
void DoublyLinkedList<T>::copy_list(const DoublyLinkedList<T>& other) {
    DoubleNodePtr<T> other_current = other.head;
    while(other_current) {
        this->append(other_current->data);
        other_current = other_current->next;
    }
}

template <typename T>
void DoublyLinkedList<T>::copy_list(const LinkedList<T>& other) {
    NodePtr<T> other_current = other.head;
    while(other_current) {
        this->append(other_current->data);
        other_current = other_current->next;
    }
}

template <typename T>
void DoublyLinkedList<T>::clear() {
    while(head) {
        DoubleNodePtr<T> temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
void DoublyLinkedList<T>::append(T data) {
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
void DoublyLinkedList<T>::insert(T data, const size_t idx) {
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
T DoublyLinkedList<T>::remove(const size_t idx) {
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

template <>
void DoublyLinkedList<Student>::obliterate_student(const size_t idx, const char* csv_path) {
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
size_t DoublyLinkedList<T>::len() {
    return this->size;
}

template <typename T>
void DoublyLinkedList<T>::sort(CompareFn<T> compare_fn) {
    this->head = this->merge_sort(this->head, compare_fn);
}

template <typename T>
DoubleNodePtr<T> DoublyLinkedList<T>::merge_sort(DoubleNodePtr<T> head, CompareFn<T> compare_fn) {
    if(!head || !head->next) {
        return head;
    }

    DoubleNodePtr<T> slow = head;
    DoubleNodePtr<T> fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    DoubleNodePtr<T> right = slow->next;
    slow->next = nullptr;

    head = merge_sort(head, compare_fn);
    right = merge_sort(right, compare_fn);

    return merge(head, right, compare_fn);
}

template <typename T>
DoubleNodePtr<T> DoublyLinkedList<T>::merge(DoubleNodePtr<T> left, DoubleNodePtr<T> right, CompareFn<T> compare_fn) {
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

namespace DoublyLinkedListFn {
    DoublyLinkedList<string> from_row(string row) {
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

template DoublyLinkedList<Student>::DoublyLinkedList();
template DoublyLinkedList<Student>::DoublyLinkedList(const LinkedList<Student>& other);
template DoublyLinkedList<Student>::~DoublyLinkedList();
template Student& DoublyLinkedList<Student>::operator[](size_t idx);
template void DoublyLinkedList<Student>::append(Student data);
template Student DoublyLinkedList<Student>::remove(size_t idx);
template size_t DoublyLinkedList<Student>::len();
template void DoublyLinkedList<Student>::sort(CompareFn<Student> compare_fn);

template string& DoublyLinkedList<string>::operator[](size_t idx);
template DoublyLinkedList<string>::~DoublyLinkedList();
template void DoublyLinkedList<string>::sort(CompareFn<string> compare_fn);
template void DoublyLinkedList<string>::insert(string data, const size_t idx);
template string DoublyLinkedList<string>::remove(const size_t idx);