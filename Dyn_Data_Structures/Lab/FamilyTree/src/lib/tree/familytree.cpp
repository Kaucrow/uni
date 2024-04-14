#include "familytree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../../../../Stack/src/lib/misc/panic/panic.h"
#include "../../../../Linked_List/src/linked_list/linked_list.h"

using std::ifstream, std::string, std::getline, std::cout, std::stringstream;

Gender char_to_gender(char c) {
    switch (c) {
        case 'F': return Gender::F; break;
        case 'M': return Gender::M; break;
    };
}

void PersonNode::marry(PersonNodePtr spouse) {
    this->spouse = spouse;
    spouse->spouse = this;
    spouse->bloodrel = false;

    if (this->gender == Gender::M) {
        spouse->parent = this->parent;
        spouse->children = this->children;
        this->children = nullptr;

        if (this->parent) {
            this->parent->children->remove(this->parent->children->find(this));
            this->parent->children->append(spouse);
        }
    }
}

void PersonNode::add_child(PersonNodePtr child) {
    if (!this->children) {
        this->children = new LinkedList<PersonNodePtr>;
    }

    this->children->append(child);
    child->parent = this;
}

void PersonNode::divorce() {
    if (this->spouse) {
        this->spouse->spouse = nullptr;
    } else {
        throw("[ ERR ] Attempted to divorce a person without spouse.\n\tDo you have a grudge against them or something?\n");
    }
}

void PersonNode::display_cousins() {
    if (!this->parent || !this->parent->parent)
        throw("[ ERR ] There is no information about the siblings of this person's parents.");

    for (auto uncle : *this->parent->parent->children) {
        if (uncle != this->parent) {
            if (uncle->children) {
                for (auto cousin : *uncle->children) {
                    cout << "[ " << this->name << "'s cousin ] " << cousin->name << '\n';
                }
            }
        }
    }
}

PersonNode::~PersonNode() {
    if (this->parent) {
        if ((*this->parent->children)[this->parent->children->len() - 1] == this)
            delete this->children;
    } else {
        delete this->children;
    }
}

FamilyTree::FamilyTree(const char* TREEFILE_PATH) : root(nullptr), height(0), nodes_per_level("1")
{
    ifstream treefile(TREEFILE_PATH);

    if (!treefile)
        panic("[ ERR ] Could not open treefile.");

    string line;
    string field;
    PersonNodePtr prev_parent = nullptr;

    size_t next_comma;

    getline(treefile, line);
    while(getline(treefile, line)) {
        PersonNodePtr curr_parent = nullptr;
        string parent_f, spouse_f, children_f;

        stringstream ssline(line);
        for (int i = 0; getline(ssline, field, ','); i++) {
            switch (i) {
                case 0: parent_f = field; break;
                case 1: spouse_f = field; break;
                case 2: children_f = field; break;
                default: panic(); break;
            }
        }

        size_t parent_len = parent_f.length();

        if (!this->root) {
            this->root = new PersonNode(parent_f);//parent_f.substr(0, parent_len - 1), char_to_gender(parent_f[parent_len - 1]));
            curr_parent = this->root;
        } else {
            get_person_ref(parent_f.substr(0, parent_f.length() - 1), curr_parent);
            if (!curr_parent) {
                cout << "Parent: " << parent_f << '\n';
                panic("Could not find the parent in the familytree.");
            }
            /*else {
                cout << curr_parent->name << std::endl;
                panic("HERE");
            }*/
        }
        //cout << curr_parent->name << std::endl;
        curr_parent->marry(new PersonNode(spouse_f));

        if (curr_parent->gender == Gender::M) {
            curr_parent = curr_parent->spouse;
        }

        stringstream sschildren(children_f);
        string child;
        while(getline(sschildren, child, '|')) {
            curr_parent->add_child(new PersonNode(child));
        }

        /*for (auto child : *curr_parent->children) {
            cout << child->name << ' ';
        }*/
        //cout << "\n";
        //if (curr_parent->name == "Maria")
        //    panic("HERE");
        prev_parent = curr_parent;
    }
}

FamilyTree::~FamilyTree() {
    destructor_backend(this->root);
}

void FamilyTree::destructor_backend(PersonNodePtr node) {
    if (node->children)
        for(auto child : *node->children)
            destructor_backend(child);

    delete node;
}

void FamilyTree::get_person_ref(string name, PersonNodePtr &store_in) {
    store_in = nullptr;
    get_person_ref_backend(name, this->root, store_in);
    if (!store_in)
        throw("\n[ ERR ] Couldn't find the person specified.\n");
}

void FamilyTree::get_person_ref_backend(string name, PersonNodePtr node, PersonNodePtr &store_in) {
    if (node->name == name) {
        store_in = node;
        return;
    }
    
    if (node->children) {
        for(auto child : *node->children) {
            get_person_ref_backend(name, child, store_in);
            if (store_in)
                return;
        }
    }
}

void FamilyTree::display_parents_children() {
    display_parents_children_backend(this->root);
}

void FamilyTree::display_parents_children_backend(PersonNodePtr node) {
    if (node->children) {
        cout << "Mother: " << node->name << " | Father: " << node->spouse->name << '\n';
        for (auto child : *node->children) {
            cout << "  * " << child->name << '\n';
        }
        cout << '\n';

        for (auto child : *node->children)
            display_parents_children_backend(child);
    }
}

void FamilyTree::display_women(DisplayType display_type) {
    display_women_backend(display_type, this->root, 0);
}

void FamilyTree::display_women_backend(DisplayType display_type, PersonNodePtr node, int level) {
    if (node->gender == Gender::F) {
        switch (display_type) {
            case DisplayType::LEVEL:
                cout << "* " << node->name << " (Level " << level << ")\n"; break;
            case DisplayType::BLOODREL:
                cout << "* " << node->name << " (" << (node->bloodrel ? "Bloodrel" : "Non-Bloodrel") << ")\n"; break;
            default:
                panic(); break;
        }
        if (node->children)
            for (auto child : *node->children)
                display_women_backend(display_type, child, level + 1);
    }
}

void FamilyTree::display_men(DisplayType display_type) {
    display_men_backend(display_type, this->root, 0);
}

void FamilyTree::display_men_backend(DisplayType display_type, PersonNodePtr node, int level) {
    PersonNodePtr display_node = nullptr;
    if (node->gender == Gender::M) {
        display_node = node;
    } else if (node->spouse && node->spouse->gender == Gender::M) {
        display_node = node->spouse;
    }

    if (display_node) {
        switch (display_type) {
            case DisplayType::LEVEL:
                cout << "* " << display_node->name << " (Level " << level << ")\n"; break;
            case DisplayType::BLOODREL:
                cout << "* " << display_node->name << " (" << (display_node->bloodrel ? "Bloodrel" : "Non-Bloodrel") << ")\n"; break;
            default:
                panic(); break;
        }
    }

    if (node->children)
        for (auto child : *node->children)
            display_men_backend(display_type, child, level + 1);
}

void FamilyTree::display_others(DisplayType display_type) {
    display_others_backend(display_type, this->root);
}

void FamilyTree::display_others_backend(DisplayType display_type, PersonNodePtr node) {
    switch (display_type) {
        case DisplayType::CHILDLESSCOUPLE:
            if (!node->children && node->spouse)
                cout << "[ Childless couple ] Woman: " << node->name << " | Man: " << node->spouse->name << '\n'; break;

        case DisplayType::MAIDENLESS:
            if (!node->spouse) 
                cout << "[ Single ] " << node->name << '\n';
            else if (node->spouse->spouse == nullptr) {
                cout << "[ Single ] " << node->name << '\n';
                cout << "[ Single ] " << node->spouse->name << '\n';
            }
            break;

        default:
            panic(); break;
    }

    if (node->children)
        for(auto child : *node->children)
            display_others_backend(display_type, child);
}

/*FamilyTree::~FamilyTree() {
    LinkedList<PersonNodePtr> adder;

    this->get_preorder_backend(this->root, &adder);

    for (auto node : adder)
        delete node;
}*/

/*void FamilyTree::insert(PersonNodePtr data, LinkedList<int> path) {
    PersonNodePtr insert_at = this->root;

    for (auto val : path) {
        if (val >= insert_at->children.len())
            panic("Tried to insert a tree node on a non-existent position");

        insert_at = insert_at->children[val];
    }

    insert_at->children.append(data);

    if (path.len() >= this->height) {
        this->height++;
        this->nodes_per_level.append(1);
    } else
        nodes_per_level[path.len() + 1]++;
}

LinkedList<PersonNodePtr> FamilyTree::get_level(size_t level) {
    if (level > this->height)
        panic("Tried to access a non-existent tree level");

    LinkedList<PersonNodePtr> adder;

    this->get_level_backend(this->root, &adder, 0, level);

    LinkedList<PersonNodePtr> ret;

    for (auto node : adder) {
        ret.append(node->data);
    }

    return ret;
}

void FamilyTree::get_level_backend
    (
        PersonNodePtr node,
        LinkedList<PersonNodePtr>* adder,
        size_t on_level,
        size_t tgt_level
    )
{
    if (tgt_level == on_level)
        adder->append(node);

    if (node->children.len() == 0)
        return;

    for (auto child_node : node->children) {
        this->get_level_backend(child_node, adder, on_level + 1, tgt_level);
    }
}

void FamilyTree::traverse(Traversal type, LinkedList<PersonNodePtr>* ret) {
    LinkedList<PersonNodePtr> adder;

    switch (type) {
        case Traversal::PREORDER:   this->get_preorder_backend(this->root, &adder); break;
        case Traversal::INORDER:    this->get_inorder_backend(this->root, &adder); break;
        case Traversal::POSTORDER:  this->get_postorder_backend(this->root, &adder); break;
    }

    for (auto& node : adder)
        ret->append(node->data);
}

LinkedList<PersonNodePtr> FamilyTree::get_preorder() {
    LinkedList<PersonNodePtr> ret;
    this->traverse(Traversal::PREORDER, &ret);
    return ret;
}

void FamilyTree::get_preorder_backend(TRAVERSAL_ARGS) {
    adder->append(node);

    for (auto child_node : node->children)
        this->get_preorder_backend(child_node, adder);
}

LinkedList<PersonNodePtr> FamilyTree::get_inorder() {
    LinkedList<PersonNodePtr> ret;
    this->traverse(Traversal::INORDER, &ret);
    return ret;
}

void FamilyTree::get_inorder_backend(TRAVERSAL_ARGS) {
    int counter = 0;

    for (auto child_node : node->children) {
        this->get_inorder_backend(child_node, adder);
        counter++;
        if (counter == node->children.len() / 2)
            adder->append(node);
    }

    if(node->children.len() <= 1) {
        adder->append(node);
    }
}

LinkedList<PersonNodePtr> FamilyTree::get_postorder() {
    LinkedList<PersonNodePtr> ret;
    this->traverse(Traversal::POSTORDER, &ret);
    return ret;
}

void FamilyTree::get_postorder_backend(TRAVERSAL_ARGS) {
    for (auto child_node : node->children)
        this->get_postorder_backend(child_node, adder);

    adder->append(node);
}

void FamilyTree::remove(LinkedList<int> path) {
    PersonNodePtr remove_node = this->root;
    PersonNodePtr prev_node = nullptr;

    if (path.len() == 0)
        panic("Attempted to remove the tree root");

    try {
        for (auto val : path) {
            prev_node = remove_node;
            remove_node = remove_node->children[val];
        }
    } catch (...) {
        panic("Tried to remove a non-existent node");
    }

    LinkedList<PersonNodePtr> remove_node_children;

    this->get_children(remove_node, &remove_node_children);

    for (auto child_node : remove_node_children) {
        this->nodes_per_level[child_node->level]--;
        delete child_node;
    }

    this->nodes_per_level[remove_node->level]--;
    prev_node->children.remove(path[path.len() - 1]);

    delete remove_node;

    for (int i = this->nodes_per_level.len() - 1; i >= 0; i--) {
        if (this->nodes_per_level[i] == 0) {
            this->nodes_per_level.remove(i);
            this->height--;
        }
    }
}

/*template <typename T>
void FamilyTree::get_group(TRAVERSAL_ARGS) {
    adder->append(node);

    if (node->children.len() == 0)
        return;

    for (auto child_node : node->children) {
        this->get_group(child_node, adder);
    }
}*/

/*
void FamilyTree::get_children(TRAVERSAL_ARGS) {
    this->get_preorder_backend(node, adder);
    adder->remove(0);
}

LinkedList<PersonNodePtr> FamilyTree::get_direct_children(LinkedList<int> path) {
    PersonNodePtr node = this->root;

    try {
        for (auto val : path) {
            node = node->children[val];
        }
    } catch (...) {
        panic("Tried to get the children of a non-existent node");
    }

    LinkedList<PersonNodePtr> ret;

    for (auto child_node : node->children) {
        ret.append(child_node->data);
    }

    return ret;
}

size_t FamilyTree::get_height() {
    return this->height;
}

bool FamilyTree::empty() {
    return !this->root;
}*/


/*template Tree<int>::Tree(int root);
template Tree<int>::~Tree();
template void Tree<int>::insert(int data, LinkedList<int> path);
template void Tree<int>::remove(LinkedList<int> path);
template size_t Tree<int>::get_height();
template LinkedList<int> Tree<int>::get_level(size_t level);
template LinkedList<int> Tree<int>::get_preorder();
template LinkedList<int> Tree<int>::get_inorder();
template LinkedList<int> Tree<int>::get_postorder();
template LinkedList<int> Tree<int>::get_direct_children(LinkedList<int> path);
template void Tree<int>::get_level_backend
    (
        TreeNodePtr<int> node,
        LinkedList<TreeNodePtr<int>>* adder,
        size_t on_level,
        size_t tgt_level
    );
*/