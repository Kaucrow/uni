#include <iostream>
#include <string>
#include <fstream>
#include "./lib/tree/familytree.h"
#include "../../Stack/src/lib/misc/panic/panic.h"

using std::cout, std::string;
const char* TREEFILE_PATH = "./data/tree.csv";

int main() {
    cout << "[ Execution Begin ]\n\n";

    FamilyTree familytree(TREEFILE_PATH);

    PersonNodePtr person = nullptr;

    try {
        familytree.get_person_ref("Maria", person);

        if (!person)
            throw("Err");
        
    } catch (...) {
        cout << "\n[ ERR ] Couldn't find the person specified.\n";
    }

    if (person)
        try {
            person->divorce();
        } catch (const char* err) {
            cout << err;
        }

    familytree.display_parents_children();

    familytree.display_women(DisplayType::LEVEL);
    familytree.display_women(DisplayType::BLOODREL);

    cout << '\n';

    familytree.display_men(DisplayType::LEVEL);
    familytree.display_men(DisplayType::BLOODREL);

    cout << '\n';

    familytree.display_others(DisplayType::MAIDENLESS);
    familytree.display_others(DisplayType::CHILDLESSCOUPLE);

    cout << "\n[ Execution End ]\n";
}