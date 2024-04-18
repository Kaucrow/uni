#pragma once
#include <iostream>
#include <string>
#include "../../../../Linked_List/src/linked_list/linked_list.h"

using std::string, std::ostream;

template <typename T>
struct GraphNode;

template <typename T>
using GraphNodePtr = GraphNode<T>*;

template <typename T>
ostream& operator<<(ostream &os, const GraphNodePtr<T> &graphnodeptr);

template <typename T>
struct GraphNodeEdge {
    GraphNodeEdge(GraphNodePtr<T> node_to, int weight) : node_to(node_to), weight(weight) {}
    GraphNodeEdge(const GraphNodeEdge &other);

    GraphNodePtr<T> node_to;
    int weight;
};

template <typename T>
ostream& operator<<(ostream &os, const GraphNodeEdge<T> &edge);

enum RoomProp {
    IS_START,
    IS_END,
    IS_KEY,
    NONEPROP
};

RoomProp RoomProp_from_str(string str);

struct Room {
    Room(int number) : number(number), prop(RoomProp::NONEPROP) {}
    Room(int number, RoomProp prop) : number(number), prop(prop) {}
    bool operator == (const Room &other);

    int number;
    RoomProp prop;
};
    
ostream& operator<<(ostream &os, const Room &room);

template <typename T>
struct GraphNode {
    GraphNode(T data) : data(data) {}

    T data;
    LinkedList<GraphNodeEdge<T>> edges;
};

template <typename T>
struct DijkstraRow {
    DijkstraRow(GraphNodePtr<T> node, int dist_origin) : node(node), dist_origin(dist_origin), prev_node(nullptr) {}

    GraphNodePtr<T> node;
    int dist_origin;
    GraphNodePtr<T> prev_node;
};

template <typename T>
ostream& operator<<(ostream &os, const DijkstraRow<T> &row);

template <typename T>
struct DijkstraMap {
    void const display();

    GraphNodePtr<T> origin;
    LinkedList<DijkstraRow<T>> data;
    /*class Iterator {
        public:
            Iterator(DijkstraRow<T> start);
            Iterator& operator++();
            bool operator!=(const Iterator& other) const;
            DijkstraRow<T>& operator*() const;
        private:
            DijkstraRow<T> current;
    };
    Iterator begin() const;
    Iterator end() const;*/
};

template <typename T>
class Graph {
    public:
        void add_node(T data);
        void add_edge(T from, T to, int weight = 0);
        void add_edge(GraphNodePtr<T> from, GraphNodePtr<T> to, int weight = 0);
        void const display();
        void connect_components();
        GraphNodePtr<T> get_node(T data);
        DijkstraMap<T> const gen_dijkstra_map(GraphNodePtr<T> from);
        LinkedList<GraphNodePtr<T>> const get_shortest_distance(T from, T to);
        LinkedList<GraphNodePtr<T>> const get_shortest_distance(GraphNodePtr<T> from, GraphNodePtr<T> to);
        size_t len();
    private:
        LinkedList<GraphNodePtr<T>> data;
        size_t nodes_amt;
};