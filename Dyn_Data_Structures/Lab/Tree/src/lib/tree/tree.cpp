#include "tree.h"
#include <iostream>
#include "../../../../Stack/src/lib/misc/panic/panic.h"
#include "../../../../Linked_List/src/linked_list/linked_list.h"

template <typename T>
Tree<T>::Tree(T root) : root(new TreeNode(root, 0)), height(0), nodes_per_level("1")
{}

template <typename T>
Tree<T>::~Tree() {
    LinkedList<TreeNodePtr<T>> adder;

    this->get_group(this->root, &adder);

    for (auto node : adder)
        delete node;
}

template <typename T>
void Tree<T>::insert(T data, LinkedList<int> path) {
    TreeNodePtr<T> insert_at = this->root;

    for (auto val : path) {
        if (val >= insert_at->children.len())
            panic("Tried to insert a tree node on a non-existent position");

        insert_at = insert_at->children[val];
    }

    insert_at->children.append(new TreeNode(data, path.len() + 1));

    if (path.len() >= this->height) {
        this->height++;
        this->nodes_per_level.append(1);
    } else
        nodes_per_level[path.len() + 1]++;
}

template <typename T>
LinkedList<T> Tree<T>::get_level(size_t level) {
    if (level > this->height)
        panic("Tried to access a non-existent tree level");

    LinkedList<TreeNodePtr<T>> adder;

    this->get_level_backend(this->root, &adder, 0, level);

    LinkedList<T> ret;

    for (auto node : adder) {
        ret.append(node->data);
    }

    return ret;
}

template <typename T>
void Tree<T>::get_level_backend
    (
        TreeNodePtr<T> node,
        LinkedList<TreeNodePtr<T>>* adder,
        size_t on_level,
        size_t tgt_level
    )
{
    if (tgt_level == on_level)
        adder->append(node);

    if (node->children.len() == 0)
        return;

    for (auto child_node : node->children) {
        this->get_level_backend(child_node, adder, on_level + 1, tgt_level);
    }
}

template <typename T>
LinkedList<T> Tree<T>::get_preorder() {
    LinkedList<TreeNodePtr<T>> adder;

    this->get_preorder_backend(this->root, &adder);

    LinkedList<T> ret;

    for (auto node : adder) {
        ret.append(node->data);
    }

    return ret;
}

template <typename T>
void Tree<T>::get_preorder_backend(TRAVERSAL_ARGS) {
    adder->append(node);

    for (auto child_node : node->children)
        this->get_preorder_backend(child_node, adder);
}

template <typename T>
LinkedList<T> Tree<T>::get_inorder() {
    LinkedList<TreeNodePtr<T>> adder;

    this->get_inorder_backend(this->root, &adder);

    LinkedList<T> ret;

    for (auto node : adder) {
        ret.append(node->data);
    }

    return ret;
}

template <typename T>
void Tree<T>::get_inorder_backend(TRAVERSAL_ARGS) {
    int counter = 0;

    for (auto child_node : node->children) {
        this->get_inorder_backend(child_node, adder);
        counter++;
        if (counter == node->children.len() / 2)
            adder->append(node);
    }

    if(node->children.len() <= 1) {
        adder->append(node);
    }
}

template <typename T>
LinkedList<T> Tree<T>::get_postorder() {
    LinkedList<TreeNodePtr<T>> adder;

    this->get_postorder_backend(this->root, &adder);

    LinkedList<T> ret;

    for (auto node : adder) {
        ret.append(node->data);
    }

    return ret;
}

template <typename T>
void Tree<T>::get_postorder_backend(TRAVERSAL_ARGS) {
    for (auto child_node : node->children)
        this->get_postorder_backend(child_node, adder);

    adder->append(node);
}

template <typename T>
void Tree<T>::remove(LinkedList<int> path) {
    TreeNodePtr<T> remove_node = this->root;
    TreeNodePtr<T> prev_node = nullptr;

    if (path.len() == 0) {
        panic("Attempted to remove the tree root");
    }

    try {
        for (auto val : path) {
            prev_node = remove_node;
            remove_node = remove_node->children[val];
        }
    } catch (...) {
        panic("Tried to remove a non-existent node");
    }

    LinkedList<TreeNodePtr<T>> remove_node_children;

    this->get_children(remove_node, &remove_node_children);

    for (auto child_node : remove_node_children) {
        this->nodes_per_level[child_node->level]--;
        delete child_node;
    }

    this->nodes_per_level[remove_node->level]--;
    prev_node->children.remove(path[path.len() - 1]);

    delete remove_node;

    for (int i = this->nodes_per_level.len() - 1; i >= 0; i--) {
        if (this->nodes_per_level[i] == 0) {
            this->nodes_per_level.remove(i);
            this->height--;
        }
    }
}

template <typename T>
void Tree<T>::get_group(TRAVERSAL_ARGS) {
    adder->append(node);

    if (node->children.len() == 0)
        return;

    for (auto child_node : node->children) {
        this->get_group(child_node, adder);
    }
}

template <typename T>
void Tree<T>::get_children(TRAVERSAL_ARGS) {
    this->get_group(node, adder);
    adder->remove(0);
}

template <typename T>
LinkedList<T> Tree<T>::get_direct_children(LinkedList<int> path) {
    TreeNodePtr<T> node = this->root;

    try {
        for (auto val : path) {
            node = node->children[val];
        }
    } catch (...) {
        panic("Tried to get the children of a non-existent node");
    }

    LinkedList<T> ret;

    for (auto child_node : node->children) {
        ret.append(child_node->data);
    }

    return ret;
}

template <typename T>
size_t Tree<T>::get_height() {
    return this->height;
}

template Tree<int>::Tree(int root);
template Tree<int>::~Tree();
template void Tree<int>::insert(int data, LinkedList<int> path);
template void Tree<int>::remove(LinkedList<int> path);
template size_t Tree<int>::get_height();
template LinkedList<int> Tree<int>::get_level(size_t level);
template LinkedList<int> Tree<int>::get_preorder();
template LinkedList<int> Tree<int>::get_inorder();
template LinkedList<int> Tree<int>::get_postorder();
template LinkedList<int> Tree<int>::get_direct_children(LinkedList<int> path);
template void Tree<int>::get_level_backend
    (
        TreeNodePtr<int> node,
        LinkedList<TreeNodePtr<int>>* adder,
        size_t on_level,
        size_t tgt_level
    );