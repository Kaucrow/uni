#pragma once
#include <string>

using std::string;

enum Sender {Principal, VicePrincipal, Teacher, TeacherAlt, Admin, Student};

struct Request {
    Request(string req, char delim);
    Sender sender;
    string title, description, content;
};

string sender_to_string(Sender sender);

Sender string_to_sender(string sender);