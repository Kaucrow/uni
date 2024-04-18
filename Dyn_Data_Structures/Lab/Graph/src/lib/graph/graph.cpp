#include "graph.h"
#include <iostream>
#include "../../../../Stack/src/lib/misc/panic/panic.h"

using std::cout, std::cerr;

/*template <typename T>
DijkstraMap<T>::Iterator::Iterator(DijkstraRow<T> start) : current(start) {}

template <typename T>
typename DijkstraMap<T>::Iterator& DijkstraMap<T>::Iterator::operator++() {
    if (current) {
        current = current->next;
    }
    return *this;
}

template <typename T>
bool DijkstraMap<T>::Iterator::operator!=(const DijkstraMap<T>::Iterator& other) const {
    return current != other.current;
}

template <typename T>
DijkstraRow<T>& DijkstraMap<T>::Iterator::operator*() const {
    return current->data;
}

template <typename T>
typename DijkstraMap<T>::Iterator DijkstraMap<T>::begin() const {
    return DijkstraMap<T>::Iterator(this->head);
}

template <typename T>
typename DijkstraMap<T>::Iterator DijkstraMap<T>::end() const {
    return DijkstraMap<T>::Iterator(nullptr);
}*/

template <typename T>
void const DijkstraMap<T>::display() {
    for (auto row : this->data) {
        cout << row << '\n';
    }
}

template <typename T>
ostream& operator<<(ostream &os, const DijkstraRow<T> &row) {
    os << "NODE: " << row.node << " DIST: " << row.dist_origin << " PREV: ";
    if (row.prev_node)
        os << row.prev_node;
    return os;
}

template <typename T>
ostream& operator<<(ostream &os, const GraphNodePtr<T> &nodeptr) {
    os << nodeptr->data;
    return os;
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
LinkedList<GraphNodePtr<T>> const Graph<T>::get_shortest_distance(T from, T to) {
    return this->get_shortest_distance(this->get_node(from), this->get_node(to));
}

template <typename T>
LinkedList<GraphNodePtr<T>> const Graph<T>::get_shortest_distance(GraphNodePtr<T> from, GraphNodePtr<T> to) {
    try {
        this->gen_dijkstra_map(from);
        LinkedList<GraphNodePtr<T>> ret;
        return ret;
    } catch (...) {
        cerr << "[ ERR ] Couldn't find a path from node " << from->data << "to node " << to->data << ".\n";
        panic();
    }
}

template <typename T>
struct MapGenErr {
    MapGenErr(GraphNodePtr<T> from, GraphNodePtr<T> to) : node_from(from), node_to(to) {}
    GraphNodePtr<T> node_from;
    GraphNodePtr<T> node_to;
};

template <typename T>
void Graph<T>::connect_components() {
    bool gen_success = false;
    while (!gen_success)
        try {
            this->gen_dijkstra_map(this->data[0]);
            gen_success = true;
        } catch(MapGenErr<T> err) {
            cout << "*** CONNECTED COMPONENTS ***\n";
            this->add_edge(err.node_from, err.node_to, 1);
        }
}

template <typename T>
DijkstraMap<T> const Graph<T>::gen_dijkstra_map(GraphNodePtr<T> from) {
    LinkedList<GraphNodePtr<T>> unvisited;
    LinkedList<GraphNodePtr<T>> visited;
    DijkstraMap<T> map;
    map.origin = from;

    auto get_min_dist_node = [&]() -> GraphNodePtr<T> {
        GraphNodePtr<T> min_node = nullptr;
        int min_dist = INT_MAX;
        for (auto row : map.data) {
            if (row.dist_origin < min_dist && visited.find(row.node) == -1) {
                min_node = row.node;
                min_dist = row.dist_origin;
            }
        }

        if (!min_node) {
            //panic("[ ERR ] Couldn't find the minimum distance node.");
            GraphNodePtr<T> min_edges_node = visited[0];
            for (auto node : visited)
                if (node->edges.len() < min_edges_node->edges.len())
                    min_edges_node = node;

            throw(MapGenErr(min_edges_node, unvisited[0]));
        }

        return min_node;
    };

    for (auto node : this->data) {
        if (node == from)
            map.data.append(DijkstraRow(node, 0));
        else
            map.data.append(DijkstraRow(node, INT_MAX));

        unvisited.append(node);
    }

    GraphNodePtr<T> prev_node = nullptr;
    while (unvisited.len() > 0) {
        GraphNodePtr<T> curr_node = nullptr;
        if (!prev_node) curr_node = from;
        else curr_node = get_min_dist_node();

        unvisited.remove(unvisited.find(curr_node));
        visited.append(curr_node);

        const DijkstraRow<T> &curr_node_row = map.data[curr_node->data.number - 1];
        for (auto edge : curr_node->edges) {
            DijkstraRow<T> &node_to_row = map.data[edge.node_to->data.number - 1];
            if
            (
                edge.weight + curr_node_row.dist_origin <
                node_to_row.dist_origin &&
                visited.find(edge.node_to) == -1
            )
            {
                node_to_row.dist_origin = edge.weight + curr_node_row.dist_origin;
                node_to_row.prev_node = curr_node;
            }
        }

        prev_node = curr_node;
    }

    map.display();

    return map;
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
template void Graph<Room>::connect_components();
template DijkstraMap<Room> const Graph<Room>::gen_dijkstra_map(GraphNodePtr<Room> from);
template LinkedList<GraphNodePtr<Room>> const Graph<Room>::get_shortest_distance(Room from, Room to);
template LinkedList<GraphNodePtr<Room>> const Graph<Room>::get_shortest_distance(GraphNodePtr<Room> from, GraphNodePtr<Room> to);

/*template DijkstraMap<Room>::Iterator& DijkstraMap<Room>::Iterator::operator++();
template DijkstraRow<Room>& DijkstraMap<Room>::Iterator::operator*() const;
template bool DijkstraMap<Room>::Iterator::operator!=(const DijkstraMap<Room>::Iterator& other) const;
template DijkstraMap<Room>::Iterator DijkstraMap<Room>::begin() const;
template DijkstraMap<Room>::Iterator DijkstraMap<Room>::end() const;*/