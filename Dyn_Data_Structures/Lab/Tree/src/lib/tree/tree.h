#pragma once
#include <cstddef>
#include "../../../../Linked_List/src/linked_list/linked_list.h"

#define TRAVERSAL_ARGS TreeNodePtr<T> node, LinkedList<TreeNodePtr<T>>* adder

enum Traversal {
    PREORDER,
    INORDER,
    POSTORDER
};

template<typename T>
class LinkedList;

template <typename T>
struct TreeNode {
    using TreeNodePtr = TreeNode*;

    T data;
    int level;
    LinkedList<TreeNodePtr> children;
    
    TreeNode(T data, int level) : data(data), level(level) {}
    TreeNode() {}
};

template <typename T>
using TreeNodePtr = TreeNode<T>*;

template <typename T>
class Tree {
    public:
        Tree(T root);
        ~Tree();
        void insert(T data, LinkedList<int> path);
        void remove(LinkedList<int> path);
        size_t get_height();
        LinkedList<T> get_level(size_t level);
        LinkedList<T> get_direct_children(LinkedList<int> path);
        LinkedList<T> get_preorder();
        LinkedList<T> get_inorder();
        LinkedList<T> get_postorder();
        bool empty();
    private:
        TreeNodePtr<T> root;
        size_t height;
        LinkedList<int> nodes_per_level;
        
        void get_level_backend
            (
                TreeNodePtr<T> node,
                LinkedList<TreeNodePtr<T>>* adder,
                size_t on_level,
                size_t tgt_level
            );

        void traverse(Traversal type, LinkedList<T>* ret);

        void get_preorder_backend(TRAVERSAL_ARGS);
        void get_inorder_backend(TRAVERSAL_ARGS);
        void get_postorder_backend(TRAVERSAL_ARGS);

        void get_children(TRAVERSAL_ARGS);
};