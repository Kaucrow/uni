#include <iostream> // debug
#include <fstream>
#include <string>
#include "../../misc/panic.h"
#include "student.h"

using std::string, std::getline, std::ifstream;

Student::Student() {}

Student::Student(
            int ci, string first_name, string last_name,
            string email_addr, Gender gender, int av_grade,
            int curr_year, char section, int csv_pos
        ) :
            ci(ci), first_name(first_name), last_name(last_name),
            email(email_addr), gender(gender), av_grade(av_grade),
            curr_year(curr_year), section(section), csv_pos(csv_pos)
{}

namespace StudentFn {
    Student from_row(string row) {
        Student student;

        LinkedList<string> fields = LinkedListFn::from_row(row);
        int field_num = 0;
        for(auto const &field : fields) {
            try {
                switch(field_num) {
                    case 0:
                        student.ci = stoi(field); break;
                    case 1:
                        student.first_name = field; break;
                    case 2:
                        student.last_name = field; break;
                    case 3:
                        student.email = field; break;
                    case 4:
                        if(field == "M")
                            student.gender = Gender::Male;
                        else if(field == "F")
                            student.gender = Gender::Female;
                        else
                            throw(nullptr);
                        break;
                    case 5:
                        student.av_grade = stof(field); break;
                    case 6:
                        student.curr_year = stoi(field); break;
                    case 7:
                        student.section = field[0]; break;
                    default:
                        throw(nullptr); break;
                }
            } catch(...) {
                panic("Found invalid field " + field + " in position number " + std::to_string(field_num));
            }
            field_num++;
        }

        return student;
    }

    LinkedList<Student> list_from_csv(const char* csv_path) {
        LinkedList<Student> linked_list;

        ifstream csvfile(csv_path);
        string reading_line;
        getline(csvfile, reading_line);

        while(getline(csvfile, reading_line))
            linked_list.append(StudentFn::from_row(reading_line));

        return linked_list;
    }
}