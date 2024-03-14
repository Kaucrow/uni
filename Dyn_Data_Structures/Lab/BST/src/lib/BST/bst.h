#pragma once
#include <cstddef>
#include "../../../../Linked_List/src/linked_list/linked_list.h"

template <typename T>
struct BSTNode {
    using BSTNodePtr = BSTNode;

    T key;
    BSTNodePtr right;
    BSTNodePtr left;

    BSTNode(T key) : key(key) {}
};

template <typename T>
class BST {
    public:
        Tree(T root);
        ~Tree();
    private:
        size_t height;
        LinkedList<int> nodes_per_level; 
};