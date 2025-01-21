#include <iostream> // debug
#include <fstream>
#include <string>
#include "../../misc/panic.h"
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

bool Student::operator == (const Student &other) {
    return true;
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

        ifstream csvfile;
        try {
            csvfile.open(csv_path);
            if(!csvfile)
                throw(nullptr);
        } catch(...) {
            panic("[ ERR ] Could not open the .csv file using path \'" + string(csv_path) + "\'.");
        }

        string reading_line;
        getline(csvfile, reading_line);

        while(getline(csvfile, reading_line))
            linked_list.append(StudentFn::from_row(reading_line, linked_list.len() + 2));

        return linked_list;
    }
}

template <>
void LinkedList<Student>::obliterate_student(const size_t idx, const char* csv_path) {
    Student deleted = this->remove(idx);
    int csv_remove_line = deleted.csv_pos;

    string csv_path_str = string(csv_path);
    ifstream infile(csv_path_str);

    string tempfile_path = csv_path_str.substr(0, csv_path_str.rfind('/') + 1) + "temp.csv";
    ofstream tempfile(tempfile_path);

    string reading_line;
    while(getline(infile, reading_line)) {
        if(csv_remove_line == 1) {
            csv_remove_line--;
            continue;
        }
        tempfile << reading_line << '\n';
        csv_remove_line--;
    }

    infile.close();
    tempfile.close();
    
    std::remove(csv_path);
    std::rename(tempfile_path.c_str(), csv_path);
}