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
    linked_list.sort();
    for(auto value : linked_list) {
        cout << value << '\n';
    }

    LinkedList<Student> students = StudentFn::list_from_csv("C:/Users/Javier/Desktop/Javier/Code/Uni/Dyn_Data_Structures/Lab/Linked_List/data/students.csv");

    students.sort([](const Student& a, const Student& b) {
        return a.av_grade < b.av_grade;
    });

    size_t count = 0;
    LinkedList<Student> top_10_students;
    for(auto student : students) {
        if(students.len() - 10 <= count)
            top_10_students.append(student);
        else
            count++;

    }

    for(auto student : top_10_students) {
        cout << student.first_name << ' ' << student.last_name << " (" << student.av_grade << ")\n";
    }

    students[0].write_to_file("C:/Users/Javier/Desktop/Javier/Code/Uni/Dyn_Data_Structures/Lab/Linked_List/data/");

    cout << "Terminating execution... :)\n";
}