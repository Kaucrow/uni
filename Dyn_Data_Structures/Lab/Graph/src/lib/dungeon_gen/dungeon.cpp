#include <iostream>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include "dungeon.h"
#include "../../../../Stack/src/lib/misc/panic/panic.h"
#include "../../../../../../Tools/Cpp/random_gen.h"

using std::ofstream, std::cout;

namespace fs = std::filesystem;

#define DATA_FOLDER "..\\..\\..\\data"

Graph<Room> gen_dungeon_level(DungeonProperties prop) {
    Graph<Room> level;

    for (int i = 1; i <= prop.room_amt; i++)
        level.add_node(Room(i));

    if (prop.max_conn > prop.room_amt)
        panic("[ ERR ] The level layout file cannot be generated safely.\n\t\t   The max connections is higher than the room amount.");

    string filename = gen_level_layout_file(prop);

    return level;
}

string gen_level_layout_file(DungeonProperties &prop) {
    if (!CreateDirectory(".\\data", NULL) &&
        !(ERROR_ALREADY_EXISTS == GetLastError()))
        panic("[ ERR ] Couldn't create the data folder.");

    string data_path = "./data";

    int filenum = 1;

    for (const auto &entry : fs::directory_iterator(data_path)) {
        int new_filenum = get_file_number(entry.path().filename().generic_string()) + 1;
        if (new_filenum > filenum)
            filenum = new_filenum;
    }

    string filename = "level_" + std::to_string(filenum) + ".txt";

    ofstream out("./data/" + filename);

    int start_room = Random(1, prop.room_amt);

    int end_room = Random(1, prop.room_amt);
    while (start_room == end_room)
        end_room = Random(1, prop.room_amt);

    int key_room = Random(1, prop.room_amt);
    while (key_room == start_room || key_room == end_room)
        key_room = Random(1, prop.room_amt);

    if (!out)
        panic("[ ERR ] Couldn't create the output file.");

    out <<  "--- ROOM CONNECTIONS ---\n"
        <<  "room,connection,weight\n";

    LinkedList<LinkedList<int>> connections;

    for (int i = 1; i <= prop.room_amt; i++)
        connections.append(LinkedList<int>());

    for (int i = 1; i <= prop.room_amt; i++) {
        for (int j = 1; j <= Random(prop.min_conn, prop.max_conn); j++) {
            cout << i << '\n';
            int room_to = Random(1, prop.room_amt);

            while 
            (
                room_to == i ||
                (i == start_room && room_to == end_room) ||
                (i == end_room && room_to == start_room)
            )
                room_to = Random(1, prop.room_amt);

            if (connections[i - 1].find(room_to) == -1) {
                connections[i - 1].append(room_to);
                connections[room_to - 1].append(i);

                int weight = Random(1, 9);
                out << i << ',' << room_to << ',' << weight << '\n';
                out << room_to << ',' << i << ',' << weight << '\n';
            }
        }
    }

    out << "\n--- ROOM PROPERTIES ---\n"
        << "room,property\n"
        << start_room << ",IS_START\n"
        << end_room << ",IS_END\n"
        << key_room << ",IS_KEYROOM\n";

    return filename;
}

int get_file_number(string filename) {
    cout << filename << '\n';
    size_t first_pos = filename.find_first_of("0123456789");
    size_t last_pos = filename.find_last_of("0123456789");
    cout << filename.substr(first_pos, last_pos - first_pos + 1) << '\n';
    return stoi(filename.substr(first_pos, last_pos - first_pos + 1));
}