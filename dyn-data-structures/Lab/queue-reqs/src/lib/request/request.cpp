#include "request.h"
#include "../misc/panic/panic.h"

using std::getline;

Request::Request(string req, char delim) {
    string field;
    size_t next_delim;
    for(int i = 0; i < 4; i++) {
        next_delim = req.find(delim);
        field = req.substr(0, next_delim);
        switch(i) {
            case 0: this->sender = string_to_sender(field); break;
            case 1: this->title = field; break;
            case 2: this->description = field; break;
            case 3: this->content = field; break;
        }
        req = req.substr(next_delim + 1);
    }
};

string sender_to_string(Sender sender) {
    switch(sender) {
        case Sender::Principal:     return "Principal";
        case Sender::VicePrincipal: return "VicePrincipal";
        case Sender::Teacher:       return "Teacher";
        case Sender::TeacherAlt:    return "TeacherAlt";
        case Sender::Admin:         return "Admin";
        case Sender::Student:       return "Student";
        default: panic("[ ERR ] Could not convert type Sender to type string.");
    } 
}

Sender string_to_sender(string sender) {
    if      (sender == "Principal")     return Sender::Principal;
    else if (sender == "VicePrincipal") return Sender::VicePrincipal;
    else if (sender == "Teacher")       return Sender::Teacher;
    else if (sender == "TeacherAlt")    return Sender::TeacherAlt;
    else if (sender == "Admin")         return Sender::Admin;
    else if (sender == "Student")       return Sender::Student;
    else panic("[ ERR ] Could not convert type string to type Sender.");
}
