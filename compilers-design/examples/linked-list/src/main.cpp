#include <iostream>
#include <fstream>
#include <string>
#include "classes/student/student.h"
#include "includes.h"

using   std::string, std::cout, std::cin, std::getline,
        std::ifstream, std::ofstream;

int main() {
    cout << "Execution start :)\n\n";

    LinkedList<Student> students = StudentFn::list_from_csv("C:/Users/Javier/Desktop/Javier/Code/uni/compilers-design/examples/linked-list/data/students.csv");

    students.sort([](const Student& a, const Student& b) {
        return a.av_grade < b.av_grade;
    });

    size_t count = 1;

    cout << "*** TOP 10 GIGACHADS OF ALL TIME ***\n";
    for (auto student : students) {
        if (students.len() - 10 < count) {
            cout << count - students.len() + 10 << ". " << student.first_name << " - " << student.av_grade << '\n';
        }

        count++;
    }

    cout << "\nTerminating execution... :)\n";
}