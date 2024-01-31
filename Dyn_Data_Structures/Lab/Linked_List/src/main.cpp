#include <iostream>
#include <fstream>
#include <string>
#include "classes/student/student.h"
#include "linked_list/linked_list.h"

using   std::string, std::cout, std::cin, std::getline,
        std::ifstream, std::ofstream;

int main() {
    cout << ":) Hello world start\n";
    string hello = "hello,a";
    LinkedList<string> linked_list = LinkedListFn::from_row(hello);
    for(auto value : linked_list) {
        cout << value << '\n';
    }
    cout << ":) Hello world end\n";
}