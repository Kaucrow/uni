#include <iostream>
#include <fstream>
#include <string>
#include "lib/student/student.h"
#include "lib/linked_list/linked_list.h"
#include "lib/doubly_linked_list/doubly_linked_list.h"

using   std::string, std::cout, std::cin, std::getline,
        std::ifstream, std::ofstream;

int main() {
    cout << "Execution start :)\n\n";

    LinkedList<Student> students = StudentFn::list_from_csv("C:/Users/Javier/Desktop/Javier/Code/Uni/Dyn_Data_Structures/Lab/Doubly_Linked_List/data/students.csv");

    students.sort([](const Student& a, const Student& b) {
        return a.av_grade < b.av_grade;
    });

    size_t count = 0;
    LinkedList<Student> top_10_students;
    cout << "*** TOP 10 GIGACHADS OF ALL TIME ***\n";
    for(auto student : students) {
        if(students.len() - 10 <= count)
            top_10_students.append(student);
        else
            count++;
    }

    for(auto student : top_10_students) {
        cout << student.first_name << ' ' << student.last_name << " (" << student.av_grade << ")\n";
    }

    cout << '\n' << students[1].first_name << '\n';

    students.obliterate_student(1, "C:/Users/Javier/Desktop/Javier/Code/Uni/Dyn_Data_Structures/Lab/Doubly_Linked_List/data/students.csv");

    students[0].write_to_file("C:/Users/Javier/Desktop/Javier/Code/Uni/Dyn_Data_Structures/Lab/Doubly_Linked_List/data/");

    DoublyLinkedList<Student> double_students(students);

    for(auto student : double_students) {
        cout << student.av_grade << '\n';
    }

    for(auto it = double_students.rbegin(); it != double_students.rend(); --it) {
        cout << (*it).av_grade << '\n';
    }

    cout << "Terminating execution... :)\n";
}