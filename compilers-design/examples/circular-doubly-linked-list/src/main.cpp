#include <iostream>
#include <fstream>
#include <string>
#include "classes/student/student.h"
#include "includes.h"

using   std::string, std::cout, std::cin, std::getline,
        std::ifstream, std::ofstream;

int main() {
    cout << "Execution start :)\n\n";

    CircularDoublyLinkedList<Student> students = StudentFn::list_from_csv("C:/Users/Javier/Desktop/Javier/Code/uni/compilers-design/examples/circular-doubly-linked-list/data/students.csv");

    cout << "*** 10 STUDENTS ***\n";

    DoubleNodePtr<Student> curr = students[5];

    for (int i = 1; i <= 10; i++) {
        cout << i << ". " << curr->data.first_name << " - " << curr->data.av_grade << '\n';
        curr = curr->prev;
    }

    cout << "\nTerminating execution... :)\n";
}