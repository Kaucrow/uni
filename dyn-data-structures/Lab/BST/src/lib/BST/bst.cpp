#include "bst.h"
#include "../../../../Stack/src/lib/misc/panic/panic.h"
#include <iostream>

template <typename T>
BST<T>::BST() : root(nullptr)
{}

template <typename T>
BST<T>::BST(T root) : root(new BSTNode(root))
{}

template <typename T>
BST<T>::~BST() {
    LinkedList<BSTNodePtr<T>> adder;

    this->get_preorder_backend(this->root, &adder);

    for (auto node : adder)
        delete node;
}

template <typename T>
void BST<T>::insert(T key) {
    this->insert_backend(this->root, key);
}

template <typename T>
BSTNodePtr<T> BST<T>::insert_backend(BSTNodePtr<T> node, T key) {
    if (!node) {
        if (node == this->root) {
            this->root = new BSTNode(key);
            return this->root;
        } else
            return (new BSTNode(key));
    }

    if (key < node->key)
        node->left = insert_backend(node->left, key);
    else if (key > node->key)
        node->right = insert_backend(node->right, key);

    return node;
}

template <typename T>
void BST<T>::traverse(Traversal type, LinkedList<T>* ret) {
    LinkedList<BSTNodePtr<T>> adder;

    switch (type) {
        case Traversal::PREORDER:    this->get_preorder_backend(this->root, &adder); break;
        case Traversal::INORDER:     this->get_inorder_backend(this->root, &adder); break;
        case Traversal::POSTORDER:   this->get_postorder_backend(this->root, &adder); break;
    }

    for (auto& node : adder)
        ret->append(node->key);
}    

template <typename T>
LinkedList<T> BST<T>::get_preorder() {
    LinkedList<T> ret;
    this->traverse(Traversal::PREORDER, &ret);
    return ret;
}

template <typename T>
void BST<T>::get_preorder_backend(TRAVERSAL_ARGS) {
    if (!node)
        return;

    adder->append(node);
    this->get_preorder_backend(node->left, adder);
    this->get_preorder_backend(node->right, adder);
}

template <typename T>
LinkedList<T> BST<T>::get_inorder() {
    LinkedList<T> ret;
    this->traverse(Traversal::INORDER, &ret);
    return ret;
}

template <typename T>
void BST<T>::get_inorder_backend(TRAVERSAL_ARGS) {
    if (!node)
        return;

    this->get_inorder_backend(node->left, adder);
    adder->append(node);
    this->get_inorder_backend(node->right, adder);
}

template <typename T>
LinkedList<T> BST<T>::get_postorder() {
    LinkedList<T> ret;
    this->traverse(Traversal::POSTORDER, &ret);
    return ret;
}

template <typename T>
void BST<T>::get_postorder_backend(TRAVERSAL_ARGS) {
    if (!node)
        return;

    this->get_postorder_backend(node->left, adder);
    this->get_postorder_backend(node->right, adder);
    adder->append(node);
}

template <typename T>
LinkedList<int> BST<T>::search(T search) {
    LinkedList<int> adder;
    this->search_backend(this->root, search, &adder);
    return adder;
}

template <typename T>
void BST<T>::search_backend(BSTNodePtr<T> node, T search, LinkedList<int>* adder) {
    if(!node) {
        adder->clear();
        adder->append(-1);
        return;
    }

    if(search < node->key) {
        adder->append(0);
        this->search_backend(node->left, search, adder);
    }
    else if (search > node->key) {
        adder->append(1);
        this->search_backend(node->right, search, adder);
    }
}

template <typename T>
BSTNodePtr<T> BST<T>::get_node(LinkedList<int> path) {
    BSTNodePtr<T> ret = this->root;

    for (auto val : path) {
        if (!ret) panic("[ ERR ] The bst path provided doesn't lead to any node");

        if (val == 0)
            ret = ret->left;
        else if (val == 1)
            ret = ret->right;
        else
            panic("[ ERR ] Found non-binary value in bst node path");
    }

    return ret;
}

template <typename T>
bool BST<T>::remove(T remove) {
    LinkedList<int> remove_node_path = this->search(remove);
    if (remove_node_path.len() == 0 || remove_node_path[0] != -1) {
        BSTNodePtr<T> remove_node = this->get_node(remove_node_path);
        if (remove_node_path.len() != 0) {
            remove_node_path.remove(remove_node_path.len() - 1);
            BSTNodePtr<T> remove_node_parent = this->get_node(remove_node_path);
            LinkedList<BSTNodePtr<T>> remove_node_children;
            this->get_preorder_backend(remove_node, &remove_node_children);
            remove_node_children.remove(0);
            if (remove_node_parent->right == remove_node)
                remove_node_parent->right = nullptr;
            else
                remove_node_parent->left = nullptr;
            delete remove_node;
            for (auto& child_node : remove_node_children) {
                this->insert(child_node->key);
                delete child_node;
            }
        }
        else {
            this->~BST();
            this->root = nullptr;
        }
        return 0;
    } else
        return 1;
}

template BST<int>::BST(int root);
template BST<int>::~BST();
template void BST<int>::insert(int key);
template LinkedList<int> BST<int>::get_preorder();
template LinkedList<int> BST<int>::get_inorder();
template LinkedList<int> BST<int>::get_postorder();
template LinkedList<int> BST<int>::search(int search);
template bool BST<int>::remove(int remove);