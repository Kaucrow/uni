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

    cout << "-------------------\n";
    string numbers = "1,2,3,4,5,6,7,8,9";
    DoublyLinkedList<string> numbers_list = DoublyLinkedListFn::from_row(numbers);
    string to_append = "10";
    string to_insert = "9";
    numbers_list.append(to_append);
    numbers_list.insert(to_insert, 0);
    for(auto number : numbers_list) {
        cout << number << '\n';
    }

    if(numbers_list[0] == numbers_list[9]) {
        cout << "FOUND EQUAL NUMBERS\n";
    }

    cout << "---------------------\nREVERSED:\n";
    numbers_list.remove(0); 
    for(auto it = numbers_list.rbegin(); it != numbers_list.rend(); --it) {
        cout << (*it) << '\n';
    }

    cout << "Terminating execution... :)\n";
}