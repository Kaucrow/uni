#pragma once
#include <string>
#include "../../includes.h"

using std::string;

enum Gender {
    Male,
    Female
};

class Student {
    public:
        Student();
        Student(
            int ci, string first_name, string last_name,
            string email_addr, Gender gender, int av_grade,
            int school_year, char section, int csv_pos
        );
        void write_to_file(const char* out_dir);
        bool operator == (const Student &other);

        int ci;
        string first_name;
        string last_name;
        string email;
        Gender gender;
        float av_grade;
        int school_year;
        char section;
        int csv_pos;
};

namespace StudentFn {
    Student from_row(string row, int csv_pos);
    CircularDoublyLinkedList<Student> list_from_csv(const char* csv_path);
    void obliterate_student(CircularDoublyLinkedList<Student> &list, const size_t idx, const char* csv_path);
}