#include "linked_list.h"
#include "../classes/student/student.h"

template <typename T>
struct GraphNode {};

template <typename T>
using GraphNodePtr = GraphNode<T>*;

template <typename T>
struct DijkstraRow {
    GraphNodePtr<T> node;
    int dist_origin;
    GraphNodePtr<T> prev_node;
};

template <typename T>
struct GraphNodeEdge {
    GraphNodeEdge(GraphNodePtr<T> node_to, int weight) : node_to(node_to), weight(weight) {}
    GraphNodeEdge(const GraphNodeEdge &other);

    GraphNodePtr<T> node_to;
    int weight;
};

struct Room {};

template <typename T>
LinkedList<T>::Iterator::Iterator(NodePtr<T> start) : current(start) {}

template <typename T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++() {
    if(current) {
        current = current->next;
    }
    return *this;
}

template <typename T>
bool LinkedList<T>::Iterator::operator!=(const LinkedList<T>::Iterator& other) const {
    return current != other.current;
}

template <typename T>
T& LinkedList<T>::Iterator::operator*() const {
    return current->data;
}

template <typename T>
typename LinkedList<T>::Iterator LinkedList<T>::begin() const {
    return LinkedList<T>::Iterator(this->head);
}

template <typename T>
typename LinkedList<T>::Iterator LinkedList<T>::end() const {
    return LinkedList<T>::Iterator(nullptr);
}

template LinkedList<string>::Iterator& LinkedList<string>::Iterator::operator++();
template string& LinkedList<string>::Iterator::operator*() const;
template bool LinkedList<string>::Iterator::operator!=(const LinkedList<string>::Iterator& other) const;
template LinkedList<string>::Iterator LinkedList<string>::begin() const;
template LinkedList<string>::Iterator LinkedList<string>::end() const;

template LinkedList<int>::Iterator& LinkedList<int>::Iterator::operator++();
template int& LinkedList<int>::Iterator::operator*() const;
template bool LinkedList<int>::Iterator::operator!=(const LinkedList<int>::Iterator& other) const;
template LinkedList<int>::Iterator LinkedList<int>::begin() const;
template LinkedList<int>::Iterator LinkedList<int>::end() const;

template LinkedList<Student>::Iterator& LinkedList<Student>::Iterator::operator++();
template Student& LinkedList<Student>::Iterator::operator*() const;
template bool LinkedList<Student>::Iterator::operator!=(const LinkedList<Student>::Iterator& other) const;
template LinkedList<Student>::Iterator LinkedList<Student>::begin() const;
template LinkedList<Student>::Iterator LinkedList<Student>::end() const;

template <typename T>
struct TreeNode {};

template <typename T>
using TreeNodePtr = TreeNode<T>*;

template LinkedList<TreeNodePtr<int>>::Iterator& LinkedList<TreeNodePtr<int>>::Iterator::operator++();
template TreeNodePtr<int>& LinkedList<TreeNodePtr<int>>::Iterator::operator*() const;
template bool LinkedList<TreeNodePtr<int>>::Iterator::operator!=(const LinkedList<TreeNodePtr<int>>::Iterator& other) const;
template LinkedList<TreeNodePtr<int>>::Iterator LinkedList<TreeNodePtr<int>>::begin() const;
template LinkedList<TreeNodePtr<int>>::Iterator LinkedList<TreeNodePtr<int>>::end() const;

template <typename T>
struct BSTNode {};

template <typename T>
using BSTNodePtr = BSTNode<T>*;

template LinkedList<BSTNodePtr<int>>::Iterator& LinkedList<BSTNodePtr<int>>::Iterator::operator++();
template BSTNodePtr<int>& LinkedList<BSTNodePtr<int>>::Iterator::operator*() const;
template bool LinkedList<BSTNodePtr<int>>::Iterator::operator!=(const LinkedList<BSTNodePtr<int>>::Iterator& other) const;
template LinkedList<BSTNodePtr<int>>::Iterator LinkedList<BSTNodePtr<int>>::begin() const;
template LinkedList<BSTNodePtr<int>>::Iterator LinkedList<BSTNodePtr<int>>::end() const;

struct PersonNode;
using PersonNodePtr = PersonNode*;

template LinkedList<PersonNodePtr>::Iterator& LinkedList<PersonNodePtr>::Iterator::operator++();
template PersonNodePtr& LinkedList<PersonNodePtr>::Iterator::operator*() const;
template bool LinkedList<PersonNodePtr>::Iterator::operator!=(const LinkedList<PersonNodePtr>::Iterator& other) const;
template LinkedList<PersonNodePtr>::Iterator LinkedList<PersonNodePtr>::begin() const;
template LinkedList<PersonNodePtr>::Iterator LinkedList<PersonNodePtr>::end() const;

template LinkedList<GraphNodePtr<Room>>::Iterator& LinkedList<GraphNodePtr<Room>>::Iterator::operator++();
template GraphNodePtr<Room>& LinkedList<GraphNodePtr<Room>>::Iterator::operator*() const;
template bool LinkedList<GraphNodePtr<Room>>::Iterator::operator!=(const LinkedList<GraphNodePtr<Room>>::Iterator& other) const;
template LinkedList<GraphNodePtr<Room>>::Iterator LinkedList<GraphNodePtr<Room>>::begin() const;
template LinkedList<GraphNodePtr<Room>>::Iterator LinkedList<GraphNodePtr<Room>>::end() const;

template LinkedList<GraphNodeEdge<Room>>::Iterator& LinkedList<GraphNodeEdge<Room>>::Iterator::operator++();
template GraphNodeEdge<Room>& LinkedList<GraphNodeEdge<Room>>::Iterator::operator*() const;
template bool LinkedList<GraphNodeEdge<Room>>::Iterator::operator!=(const LinkedList<GraphNodeEdge<Room>>::Iterator& other) const;
template LinkedList<GraphNodeEdge<Room>>::Iterator LinkedList<GraphNodeEdge<Room>>::begin() const;
template LinkedList<GraphNodeEdge<Room>>::Iterator LinkedList<GraphNodeEdge<Room>>::end() const;

template LinkedList<DijkstraRow<Room>>::Iterator& LinkedList<DijkstraRow<Room>>::Iterator::operator++();
template DijkstraRow<Room>& LinkedList<DijkstraRow<Room>>::Iterator::operator*() const;
template bool LinkedList<DijkstraRow<Room>>::Iterator::operator!=(const LinkedList<DijkstraRow<Room>>::Iterator& other) const;
template LinkedList<DijkstraRow<Room>>::Iterator LinkedList<DijkstraRow<Room>>::begin() const;
template LinkedList<DijkstraRow<Room>>::Iterator LinkedList<DijkstraRow<Room>>::end() const;