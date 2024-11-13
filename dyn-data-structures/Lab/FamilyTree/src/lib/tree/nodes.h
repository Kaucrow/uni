#pragma once
#include <string>

using std::string;

template <typename T>
class LinkedList;

enum Gender {F, M};

Gender char_to_gender(char c);

struct PersonNode {
    PersonNode(string name, Gender gender) :
        spouse(nullptr), parent(nullptr), children(nullptr), name(name),
        bloodrel(true), gender(gender) {}

    PersonNode(string name, Gender gender, PersonNode* parent) :
        spouse(nullptr), parent(parent), children(nullptr), name(name),
        bloodrel(true), gender(gender) {}

    PersonNode(string fulldata) :
        spouse(nullptr), parent(nullptr), children(nullptr),
        name(fulldata.substr(0, fulldata.length() - 1)),
        gender(char_to_gender(fulldata[fulldata.length() - 1])) {}

    ~PersonNode();
 
    void marry(PersonNode* spouse);
    void add_child(PersonNode* child);
    void divorce();
    void display_cousins();

    PersonNode* spouse;
    PersonNode* parent;
    bool bloodrel;
    LinkedList<PersonNode*>* children;
    Gender gender;
    string name;
};

using PersonNodePtr = PersonNode*;