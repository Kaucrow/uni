#pragma once
#include <cstddef>
#include "../../../../Linked_List/src/linked_list/linked_list.h"

#define TRAVERSAL_ARGS BSTNodePtr<T> node, LinkedList<BSTNodePtr<T>>* adder

enum Traversal {
    PREORDER,
    INORDER,
    POSTORDER
};

template <typename T>
class LinkedList;

template <typename T>
struct BSTNode {
    using BSTNodePtr = BSTNode*;

    T key;
    BSTNodePtr right;
    BSTNodePtr left;

    BSTNode(T key) : key(key), right(nullptr), left(nullptr) {}
};

template <typename T>    
using BSTNodePtr = BSTNode<T>*;

template <typename T>
class BST {
    public:
        BST();
        BST(T root);
        ~BST();
        void insert(T key);
        LinkedList<T> get_preorder();
        LinkedList<T> get_inorder();
        LinkedList<T> get_postorder();
        LinkedList<int> search(T search);
        bool remove(T remove);
    private:
        BSTNodePtr<T> root;
        size_t height;
        LinkedList<int> nodes_per_level; 
        
        BSTNodePtr<T> insert_backend(BSTNodePtr<T> node, T key);
        void traverse(Traversal type, LinkedList<T>* ret);
        void get_preorder_backend(TRAVERSAL_ARGS);
        void get_inorder_backend(TRAVERSAL_ARGS);
        void get_postorder_backend(TRAVERSAL_ARGS);
        void search_backend(BSTNodePtr<T>, T search, LinkedList<int>* adder);
        BSTNodePtr<T> get_node(LinkedList<int> path);
};