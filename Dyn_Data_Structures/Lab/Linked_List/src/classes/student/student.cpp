#include <fstream>
#include <string>
#include "../../misc/panic.h"
#include "student.h"

using std::string, std::getline, std::ifstream;

Student::Student(
            int ci, string first_name, string last_name,
            string email_addr, Gender gender, int av_grade,
            int curr_year, char section, int csv_pos
        ) :
            ci(ci), first_name(first_name), last_name(last_name),
            email_addr(email_addr), gender(gender), av_grade(av_grade),
            curr_year(curr_year), section(section), csv_pos(csv_pos)
{}

namespace StudentFn {
    Student from_row(string row) {
        panic();
    }

    LinkedList<Student> list_from_csv(const char* csv_path) {
        LinkedList<Student> linked_list;

        ifstream csvfile(csv_path);
        string reading_line;
        getline(csvfile, reading_line);

        while(getline(csvfile, reading_line)) {
            linked_list.append(StudentFn::from_row(reading_line));
        }
    }
}