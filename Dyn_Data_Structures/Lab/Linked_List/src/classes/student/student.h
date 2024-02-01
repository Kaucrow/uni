#pragma once
#include <string>
#include "../../linked_list/linked_list.h"

using std::string;

enum Gender {
    Male,
    Female
};

struct Student {
    Student();
    Student(
        int ci, string first_name, string last_name,
        string email_addr, Gender gender, int av_grade,
        int school_year, char section, int csv_pos
    );
    void write_to_file(const char* out_dir);

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
    Student from_row(string row);
    LinkedList<Student> list_from_csv(const char* csvfile_path);
}