#pragma once
#include <cstddef>
#include "./nodes.h"
#include "../../../../Linked_List/src/linked_list/linked_list.h"

#define TRAVERSAL_ARGS PersonNodePtr node, LinkedList<PersonNodePtr>* adder

enum Traversal {
    PREORDER,
    INORDER,
    POSTORDER
};

enum DisplayType {
    LEVEL,
    BLOODREL,
    CHILDLESSCOUPLE,
    MAIDENLESS,
};

template<typename T>
class LinkedList;

class FamilyTree {
    public:
        FamilyTree(const char* TREEFILE_PATH);
        ~FamilyTree();
        void get_person_ref(string name, PersonNodePtr &store_in);
        void display_parents_children();
        void display_women(DisplayType display_type);
        void display_men(DisplayType display_type);
        void display_others(DisplayType display_type);
        void insert(PersonNodePtr data, LinkedList<int> path);
        void remove(LinkedList<int> path);
        size_t get_height();
        LinkedList<PersonNodePtr> get_level(size_t level);
        LinkedList<PersonNodePtr> get_direct_children(LinkedList<int> path);
        LinkedList<PersonNodePtr> get_preorder();
        LinkedList<PersonNodePtr> get_inorder();
        LinkedList<PersonNodePtr> get_postorder();
        bool empty();
    private:
        void get_person_ref_backend(string name, PersonNodePtr node, PersonNodePtr &store_in);
        void display_parents_children_backend(PersonNodePtr node);
        void display_women_backend(DisplayType display_type, PersonNodePtr node, int level);
        void display_men_backend(DisplayType display_type, PersonNodePtr node, int level);
        void display_others_backend(DisplayType display_type, PersonNodePtr node);
        void destructor_backend(PersonNodePtr node);

        PersonNodePtr root;
        size_t height;
        LinkedList<int> nodes_per_level;
        
        void get_level_backend
            (
                PersonNodePtr node,
                LinkedList<PersonNodePtr>* adder,
                size_t on_level,
                size_t tgt_level
            );

        void traverse(Traversal type, LinkedList<PersonNodePtr>* ret);

        void get_preorder_backend(TRAVERSAL_ARGS);
        void get_inorder_backend(TRAVERSAL_ARGS);
        void get_postorder_backend(TRAVERSAL_ARGS);

        void get_children(TRAVERSAL_ARGS);
};