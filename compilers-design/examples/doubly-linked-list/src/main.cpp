#include <iostream>
#include <fstream>
#include <string>
#include "../../../doubly-linked-list/src/doubly-linked-list/doubly_linked_list.h"
#include "./classes/student/student.h"

using   std::string, std::cout, std::cin, std::getline,
        std::ifstream, std::ofstream;

int main() {
    cout << "Execution start :)\n\n";

    DoublyLinkedList<Student> students = StudentFn::list_from_csv("C:/Users/Javier/Desktop/Javier/Code/uni/compilers-design/examples/doubly-linked-list/data/students.csv");

    students.sort([](const Student& a, const Student& b) {
        return a.av_grade < b.av_grade;
    });

    size_t count = 1;

    cout << "*** TOP 10 GIGACHADS OF ALL TIME ***\n";

    for (auto it = students.rbegin(); it != students.rend(); --it) {
        std::cout << count << ". " << (*it).first_name << " - " << (*it).av_grade << '\n';
        if (count == 10) break;
        count++;
    }

    cout << "\nTerminating execution... :)\n";
}