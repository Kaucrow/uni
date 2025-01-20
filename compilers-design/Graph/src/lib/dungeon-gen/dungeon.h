#pragma once
#include "../graph/graph.h"

struct DungeonProperties {
    DungeonProperties(int room_amt, int min_conn, int max_conn) :
        room_amt(room_amt), min_conn(min_conn), max_conn(max_conn) {}

    int room_amt;
    int min_conn;
    int max_conn;
};

Graph<Room> gen_dungeon_level(DungeonProperties prop);
string gen_level_layout_file(DungeonProperties &prop);
int get_file_number(string filename);