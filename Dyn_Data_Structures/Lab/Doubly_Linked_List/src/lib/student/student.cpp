#include <iostream> // debug
#include <fstream>
#include <string>
#include "../misc/panic.h"
#include "student.h"

using std::string, std::getline, std::ifstream, std::ofstream;

Student::Student() {}

Student::Student(
            int ci, string first_name, string last_name,
            string email_addr, Gender gender, int av_grade,
            int curr_year, char section, int csv_pos
        ) :
            ci(ci), first_name(first_name), last_name(last_name),
            email(email_addr), gender(gender), av_grade(av_grade),
            school_year(school_year), section(section), csv_pos(csv_pos)
{}

void Student::write_to_file(const char* out_dir) {
    ofstream outfile(out_dir + this->first_name + '_' + this->last_name + ".txt");
    outfile     << "*** STUDENT DATA FORM ***\n"
                << "C.I.: " << this->ci << '\n'
                << "First name: " << this->first_name << '\n'
                << "Last name: " << this->last_name << '\n'
                << "Email address: " << this->email << '\n';
    if(this->gender = Gender::Male)
        outfile << "Gender: M\n";
    else
        outfile << "Gender: F\n";
    outfile     << "Average grade: " << this->av_grade << '\n'
                << "School year: " << this->school_year << '\n'
                << "Section: " << this->section;
}

namespace StudentFn {
    Student from_row(string row, int csv_pos) {
        Student student;

        LinkedList<string> fields = LinkedListFn::from_row(row);
        int field_num = 0;
        try {
            student.ci = stoi(fields[0]);
            student.first_name = fields[1];
            student.last_name = fields[2];
            student.email = fields[3];
            switch(fields[4][0]) {
                case 'M': student.gender = Gender::Male; break;
                case 'F': student.gender = Gender::Female; break;
                default: throw(nullptr);
            }
            student.av_grade = stof(fields[5]);
            student.school_year = stoi(fields[6]);
            student.section = fields[7][0];
            student.csv_pos = csv_pos;
        } catch(...) {
            panic("[ ERR ] Found invalid field in row \"" + row + "\".");
        }

        return student;
    }

    LinkedList<Student> list_from_csv(const char* csv_path) {
        LinkedList<Student> linked_list;

        ifstream csvfile(csv_path);
        string reading_line;
        getline(csvfile, reading_line);

        while(getline(csvfile, reading_line))
            linked_list.append(StudentFn::from_row(reading_line, linked_list.len() + 2));

        return linked_list;
    }
}