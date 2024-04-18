#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>
#include <filesystem>
#include "dungeon.h"
#include "../../../../Stack/src/lib/misc/panic/panic.h"
#include "../../../../../../Tools/Cpp/random_gen.h"

using   std::ofstream, std::getline, std::string, std::ifstream,
        std::cout, std::stringstream;

namespace fs = std::filesystem;

#define DATA_FOLDER "..\\..\\..\\data"

enum ReadingData {
    CONNS,
    PROPS,
    NONEDATA
};

Graph<Room> gen_dungeon_level(DungeonProperties prop) {
    Graph<Room> level;

    for (int i = 1; i <= prop.room_amt; i++)
        level.add_node(Room(i));

    if (prop.max_conn > prop.room_amt)
        panic("[ ERR ] The level layout file cannot be generated safely.\n\t\t   The max connections is higher than the room amount.");

    string filename = gen_level_layout_file(prop);

    ifstream file("./data/" + filename);

    if (!file)
        panic(" [ ERR ] Couldn't open level layout file `" + filename + "`.");

    string line;
    ReadingData reading_data = ReadingData::NONEDATA;

    while (getline(file, line)) {
        if (line[0] == '-') {
            if (line == "--- ROOM CONNECTIONS ---")
                reading_data = ReadingData::CONNS;
            else if (line == "--- ROOM PROPERTIES ---")
                reading_data = ReadingData::PROPS;
            else
                panic("[ ERR ] Found unexpected data header in layout file `" + filename + "`.");

            getline(file, line);
        }
        else if (line.empty()) {
            continue;
        }
        else {
            GraphNodePtr<Room> roomptr = nullptr;

            stringstream ssline(line);
            switch (reading_data) {
                case ReadingData::CONNS:
                    int room_from, room_to, weight;
                    for (int i = 0; getline(ssline, line, ','); i++) {
                        switch (i) {
                            case 0: room_from = stoi(line); break;
                            case 1: room_to = stoi(line); break;
                            case 2: weight = stoi(line); break;
                            default: panic("[ ERR ] Unexpected `i` value in fn gen_dungeon_level()."); break;
                        }
                    }

                    level.add_edge(Room(room_from), Room(room_to), weight);

                    break;
                case ReadingData::PROPS:
                    int room;
                    RoomProp prop;
                    for (int i = 0; getline(ssline, line, ','); i++) {
                        switch (i) {
                            case 0: room = stoi(line); break;
                            case 1: prop = RoomProp_from_str(line); break;
                            default: panic("[ ERR ] Unexpected `i` value in fn gen_dungeon_level()."); break;
                        }
                    }

                    roomptr = level.get_node(room);
                    roomptr->data.prop = prop;

                    break;
                default:
                    panic("[ ERR ] The reading_data value has no assigned action in fn gen_dungeon_level().");
                    break;
            }
        }
    }
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
        for (int j = 1; j <= Random(prop.min_conn, prop.max_conn) - connections[i - 1].len(); j++) {
            int room_to = Random(1, prop.room_amt);

            while 
            (
                room_to == i ||
                connections[room_to - 1].len() >= prop.max_conn ||
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
    size_t first_pos = filename.find_first_of("0123456789");
    size_t last_pos = filename.find_last_of("0123456789");
    return stoi(filename.substr(first_pos, last_pos - first_pos + 1));
}