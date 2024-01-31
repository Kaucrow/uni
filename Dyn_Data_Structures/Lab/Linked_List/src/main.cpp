#include <iostream>
#include <fstream>
#include <string>
#include "classes/student/student.h"
#include "linked_list/linked_list.h"

using   std::string, std::cout, std::cin, std::getline,
        std::ifstream, std::ofstream;

int main() {
    cout << "Execution start :)\n";
    string hello = "hello,j,o,e";
    LinkedList<string> linked_list = LinkedListFn::from_row(hello);
    for(auto value : linked_list) {
        cout << value << '\n';
    }

    LinkedList<Student> student_list = StudentFn::list_from_csv("C:/Users/Javier/Desktop/Javier/Code/Uni/Dyn_Data_Structures/Lab/Linked_List/data/students.csv");

    for(auto value : student_list) {
        cout << value.first_name << '\n';
    }
    cout << "Terminating execution... :)\n";
}