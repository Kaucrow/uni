#include <iostream>
#include <string>
#include <fstream>
#include "./lib/graph/graph.h"
#include "./lib/dungeon_gen/dungeon.h"
#include "../../Linked_List/src/linked_list/linked_list.h"
#include "../../Stack/src/lib/misc/panic/panic.h"

using std::cout, std::string;

int main() {
    cout << "[ Execution Begin ]\n\n";

    Graph<Room> graph = gen_dungeon_level(DungeonProperties(5, 1, 3));
    cout << "GRAPH LEN: " << graph.len() << '\n';
    graph.display();

    cout << '\n';

    LinkedList<GraphNodePtr<Room>> shortest_distance = graph.get_shortest_distance(Room(1), Room(3));

    cout << "\n[ Execution End ]\n";
}