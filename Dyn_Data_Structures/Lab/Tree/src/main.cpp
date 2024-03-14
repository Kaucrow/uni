#include <iostream>
#include <string>
#include "lib/tree/tree.h"
#include "../../../../Tools/Cpp/termcolor.hpp"

using std::cout, std::cin, std::string;

using namespace termcolor;

void ClrScr();

int main() {
    int curr_level = 1;
    string input; 
    Tree<int> tree(0);
    string path;

    while (true) {
        ClrScr();

        cout << "\t *** TREE - " << yellow << "LEVEL " << curr_level << white << " ***\n\n";

        LinkedList<int> curr_node_children;
        curr_node_children = tree.get_direct_children(LinkedList<int>(path));
        if (curr_node_children.len() > 0) {
            for (auto val : curr_node_children) {
                cout << '\t' << val;
            }
        } else {
            cout << red << "\tNo nodes here..." << white;
        }

        cout << "\n\n";

        LinkedList<int> order_list = tree.get_preorder();

        cout << "* " << cyan << "Preorder: " << white;
        for (auto val : order_list)
            cout << val << ' ';
        
        cout << '\n';

        order_list = tree.get_inorder();
        cout << "* " << cyan << "Inorder: " << white;
        for (auto val : order_list)
            cout << val << ' ';

        cout << '\n';

        order_list = tree.get_postorder();
        cout << "* " << cyan << "Postorder: " << white;
        for (auto val : order_list)
            cout << val << ' ';
        
        cout << "\n\n";

        cout    << magenta << "-> " << white << "Input an action (QUIT, PREV, ADD, DEL)\n"
                << "   or select a node: ";
        cin >> input;

        for (auto& ch : input) {
            ch = toupper(ch);
        }

        if (input == "QUIT") break;

        if (input == "PREV") {
            if (curr_level == 1) {
                cout << red << "[ ERR ] " << white << "You are currently on the tree root.\n";
                cin.ignore(1);
                std::getline(cin, input);
            } else {
                curr_level--;

                size_t last_comma;
                if (last_comma = path.find(',') != string::npos)
                    path = path.substr(0, last_comma);
                else
                    path = "";
            }
        }
        else if (input == "ADD") {
            cout << magenta << "-> " << white << "Input a value: ";
            cin >> input;
            tree.insert(stoi(input), LinkedList<int>(path));
        }
        else if (input == "DEL") {
            if (curr_level == 1) {
                cout << red << "[ ERR ] " << white << "You can't remove the tree root.\n";
            }
            else {
                cout << magenta << "-> " << white << "Removing the current node...";
                curr_level--;

                tree.remove(path);

                size_t last_comma;
                if (last_comma = path.find(',') != string::npos)
                    path = path.substr(0, last_comma);
                else
                    path = "";
            }
            cin.ignore(1);
            std::getline(cin, input);
        }
        else {
            int node_idx;

            try {
                node_idx = stoi(input);
            } catch (...) {
                cout << red << "[ ERR ] " << white << "Please input a valid action.\n";
                cin.ignore(1);
                std::getline(cin, input);
                continue;
            }

            if (node_idx >= curr_node_children.len()) {
                cout << red << "[ ERR ] " << white << "That node doesn't exist.\n";
                cin.ignore(1);
                std::getline(cin, input);
            } else {
                curr_level++;
                if(!path.empty())
                    path.append(",");
                path.append(std::to_string(node_idx));
            }
        }
    }
    cout << '\n';
}

void ClrScr(){
    #ifdef _WIN32
        // If on Windows OS
        std::system("cls");
    #else
        // Assuming POSIX OS
        std::system("clear");
    #endif
}