#include <iostream>
#include <string>
#include <fstream>
#include "./lib/graph/graph.h"
#include "./lib/dungeon_gen/dungeon.h"
#include "../../Linked_List/src/linked_list/linked_list.h"
#include "../../Stack/src/lib/misc/panic/panic.h"

using std::cout, std::string;
const char* TREEFILE_PATH = "./data/tree.csv";

struct Val {
    Val(int val) : val(val) {}
    int val;
};

ostream& operator<<(ostream& os, const Val &val) {
    os << '(' << /*edge.node_to->data*/ "A" << ',' << val.val << ')';
    return os;
}

int main() {
    cout << "[ Execution Begin ]\n\n";

    Graph<Room> graph = gen_dungeon_level(DungeonProperties(5, 1, 3));
    cout << "GRAPH LEN: " << graph.len() << '\n';
    graph.display();

    cout << "\n[ Execution End ]\n";
}