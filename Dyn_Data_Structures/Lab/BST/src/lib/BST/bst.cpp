#include "bst.h"
#include <iostream>

using std::cout;

template <typename T>
BST<T>::BST(T root) : root(new BSTNode(root))
{}

template <typename T>
BST<T>::~BST() {
    LinkedList<BSTNodePtr<T>> adder;

    this->get_group(this->root, &adder);

    for (auto node : adder)
        delete node;
}

template <typename T>
void BST<T>::insert(T key) {
    this->insert_backend(this->root, key);
}

template <typename T>
BSTNodePtr<T> BST<T>::insert_backend(BSTNodePtr<T> node, T key) {
    if (!node)
        return (new BSTNode(key));

    if (key < node->key)
        node->left = insert_backend(node->left, key);
    else if (key > node->key)
        node->right = insert_backend(node->right, key);

    return node;
}

template <typename T>
LinkedList<T> BST<T>::get_preorder() {
    LinkedList<BSTNodePtr<T>> adder;

    this->get_preorder_backend(this->root, &adder);

    LinkedList<T> ret;

    for (auto node : adder) {
        ret.append(node->key);
    }

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
void BST<T>::get_group(TRAVERSAL_ARGS) {
    if (!node)
        return;

    adder->append(node);

    this->get_group(node->left, adder);
    this->get_group(node->right, adder);
}

template BST<int>::BST(int root);
template BST<int>::~BST();
template void BST<int>::insert(int key);
//template BSTNodePtr<int> BST<int>::insert_backend(BSTNodePtr<int> node, int key);
template LinkedList<int> BST<int>::get_preorder();