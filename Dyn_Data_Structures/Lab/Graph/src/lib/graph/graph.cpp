#include "graph.h"
#include <iostream>

using std::cerr;

bool Room::operator==(const Room &other) {
    if (this->number == other.number) {
        return true;
    }
    return false;
}

template <typename T>
void Graph<T>::add_node(T data) {
    this->data.append(new GraphNode(data));
}

template <typename T>
void Graph<T>::add_edge(T from, T to, int weight) {
    try {
        GraphNodePtr<T> node_from = get_node(from);
        GraphNodePtr<T> node_to = get_node(to);

        node_from->edges.append(GraphNodeEdge(node_to, weight));

    } catch (const char* err) {
        cerr << err;
    }
}

template <typename T>
void Graph<T>::add_edge(GraphNodePtr<T> node_from, GraphNodePtr<T> node_to, int weight) {
    node_from->edges.append(GraphNodeEdge(node_to, weight));
}

template <typename T>
GraphNodePtr<T> Graph<T>::get_node(T data) {
    GraphNodePtr<T> ret = nullptr;

    for (auto node : this->data)
        if (node->data == data) {
            ret = node;
            break;
        }

    if (!ret) throw("[ ERR ] Couldn't find a graph node with the specified data.\n");

    return ret;
}

template void Graph<Room>::add_node(Room data);
template void Graph<Room>::add_edge(Room from, Room to, int weight);
template void Graph<Room>::add_edge(GraphNodePtr<Room> node_from, GraphNodePtr<Room> node_to, int weight);
template GraphNodePtr<Room> Graph<Room>::get_node(Room data);