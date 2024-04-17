#include "graph.h"
#include <iostream>
#include "../../../../Stack/src/lib/misc/panic/panic.h"

using std::cout, std::cerr;

template <typename T>
ostream& operator<<(ostream &os, const GraphNodePtr<T> &graphnodeptr) {
    os << "TEMP\n";
}

template <typename T>
ostream& operator<<(ostream& os, const GraphNodeEdge<T> &edge) {
    os << '(' << edge.node_to->data << ',' << edge.weight << ')';
    return os;
}

template <typename T>
GraphNodeEdge<T>::GraphNodeEdge(const GraphNodeEdge &other) : node_to(other.node_to), weight(other.weight) {}

RoomProp RoomProp_from_str(string str) {
    for (auto &c : str)
        c = toupper(c);

    switch (str[0]) {
        case ('I'):
            if (str == "IS_START") return RoomProp::IS_START;
            else if (str == "IS_END") return RoomProp::IS_END;
            else if (str == "IS_KEYROOM") return RoomProp::IS_KEY;
            break;
        case ('N'):
            if (str == "NONE") return RoomProp::NONEPROP; 
            break;
    }

    panic("[ ERR ] Couldn't parse RoomProp from string. Unknown RoomProp string: " + str);
}

bool Room::operator==(const Room &other) {
    if (this->number == other.number) {
        return true;
    }
    return false;
}

ostream& operator<<(ostream& os, const Room& room) {
    os << room.number;
    return os;
}

template <typename T>
void Graph<T>::add_node(T data) {
    this->data.append(new GraphNode(data));
    this->nodes_amt++;
}

template <typename T>
size_t Graph<T>::len() {
    return this->nodes_amt;
}

template <typename T>
void Graph<T>::add_edge(T from, T to, int weight) {
    try {
        GraphNodePtr<T> node_from = get_node(from);
        GraphNodePtr<T> node_to = get_node(to);

        GraphNodeEdge test(node_to, weight);
        node_from->edges.append(test);
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

template <typename T>
void const Graph<T>::display() {
    bool new_node = true;
    for (const auto &node : this->data) {
        cout << node->data << ": [";
        for (const auto &edge : node->edges) {
            if (!new_node) cout << ',';
            else new_node = false;
            cout << edge;
        }
        new_node = true;
        cout << "]\n";
    }
}

template void Graph<Room>::add_node(Room data);
template void Graph<Room>::add_edge(Room from, Room to, int weight);
template void Graph<Room>::add_edge(GraphNodePtr<Room> node_from, GraphNodePtr<Room> node_to, int weight);
template GraphNodePtr<Room> Graph<Room>::get_node(Room data);
template void const Graph<Room>::display();
template size_t Graph<Room>::len();