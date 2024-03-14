#include <iostream>
#include "lib/BST/bst.h"
#include "../../../../Tools/Cpp/termcolor.hpp"

using std::cout, std::cin, std::getline;

using namespace termcolor;

void ClrScr();

int main() {
    string input;
    BST<int> bst(10);

    while (true) {
        ClrScr();

        cout << "\t *** BIN SEARCH TREE ***\n\n";

        cout << "* " << cyan << "Preorder: " << white;
        LinkedList<int> order_list = bst.get_preorder();
        for (auto val : order_list)
            cout << val << ' ';

        cout << '\n';

        cout << "* " << cyan << "Inorder: " << white;
        order_list = bst.get_inorder();
        for (auto val : order_list)
            cout << val << ' ';

        cout << '\n';

        cout << "* " << cyan << "Postorder: " << white;
        order_list = bst.get_postorder();
        for (auto val : order_list)
            cout << val << ' ';
        
        cout << "\n\n";

        cout << magenta << "-> " << white << "Input an action (QUIT, ADD, DEL, SEARCH): ";
        cin >> input;

        for (auto& ch : input)
            ch = toupper(ch);
        
        if (input == "QUIT") break;

        if (input == "ADD") {
            cout << magenta << "-> " << white << "Input a value: ";
            cin >> input;
            bst.insert(stoi(input));
        }
        else if (input == "DEL") {
            cout << magenta << "-> " << white << "Input a value: ";
            cin >> input;

            if (bst.remove(stoi(input)) == 0)
                cout << magenta << "-> " << white << "Removing node...";
            else
                cout << red << "[ ERR ] " << white << "The node was not found.";

            cin.ignore(1);
            getline(cin, input);
        }
        else if (input == "SEARCH") {
            cout << magenta << "-> " << white << "Input a value: ";
            cin >> input;
            LinkedList<int> path = bst.search(stoi(input));

            if (path.len() == 0 || path[0] != -1) {
                cout << "* " << cyan << "Path: " << white;
                for (auto val : path)
                    cout << val << ' ';
            }
            else {
                cout << red << "[ ERR ] " << white << "The node was not found.";
            }

            cin.ignore(1);
            getline(cin, input);
        }
        else {
            cout << red << "[ ERR ] " << white << "Please input a valid action.";
            cin.ignore(1);
            getline(cin, input);
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