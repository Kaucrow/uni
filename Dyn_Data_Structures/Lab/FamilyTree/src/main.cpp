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
        person->divorce();
    } catch (const char* err) {
        cout << err;
    }

    cout << " *** PARENTS AND BIOLOGICAL CHILDREN ***\n";
    familytree.display_parents_children();

    cout << " *** WOMEN BY LEVEL***\n";
    familytree.display_women(DisplayType::LEVEL);

    cout << "\n *** WOMEN BY BLOODREL ***\n";
    familytree.display_women(DisplayType::BLOODREL);

    cout << "\n *** MEN BY LEVEL ***\n";
    familytree.display_men(DisplayType::LEVEL);

    cout << "\n *** MEN BY BLOODREL ***\n";
    familytree.display_men(DisplayType::BLOODREL);

    cout << "\n *** SINGLE PEOPLE ***\n";
    familytree.display_others(DisplayType::MAIDENLESS);
    
    cout << "\n *** COUPLES WITHOUT CHILDREN ***\n";
    familytree.display_others(DisplayType::CHILDLESSCOUPLE);

    cout << "HERE\n";
    try {
        familytree.get_person_ref("Emma", person);
        person->display_cousins();
    } catch (const char* err) {
        cout << err;
    }
    
    cout << "\n[ Execution End ]\n";
}