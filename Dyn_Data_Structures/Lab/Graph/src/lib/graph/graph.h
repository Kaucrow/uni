#pragma once
#include <string>
#include "../../../../Linked_List/src/linked_list/linked_list.h"

using std::string;

template <typename T>
struct GraphNode;

template <typename T>
using GraphNodePtr = GraphNode<T>*;

template <typename T>
struct GraphNodeEdge {
    GraphNodeEdge(GraphNodePtr<T> node_to, int weight) : node_to(node_to), weight(weight) {}

    GraphNodePtr<T> node_to;
    int weight;
};

struct Room {
    Room(int number) : number(number) {}
    bool operator == (const Room &other);

    int number;
};

template <typename T>
struct GraphNode {
    GraphNode(T data) : data(data) {}

    T data;
    LinkedList<GraphNodeEdge<T>> edges;
};

template <typename T>
class Graph {
    public:
        void add_node(T data);
        void add_edge(T from, T to, int weight = 0);
        void add_edge(GraphNodePtr<T> from, GraphNodePtr<T> to, int weight = 0);
        GraphNodePtr<T> get_node(T data);
    private:
        LinkedList<GraphNodePtr<T>> data;
};