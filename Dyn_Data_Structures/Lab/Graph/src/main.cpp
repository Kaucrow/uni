#include <iostream>
#include <string>
#include <fstream>
#include "./lib/graph/graph.h"
#include "./lib/dungeon_gen/dungeon.h"
#include "../../Stack/src/lib/misc/panic/panic.h"

using std::cout, std::string;
const char* TREEFILE_PATH = "./data/tree.csv";

int main() {
    cout << "[ Execution Begin ]\n\n";

    Graph<Room> graph;

    graph.add_node(Room(1));
    graph.add_node(Room(3));
    graph.add_edge(graph.get_node(Room(1)), graph.get_node(Room(1)), 3);

    gen_dungeon_level(DungeonProperties(5, 1, 3));

    cout << "\n[ Execution End ]\n";
}