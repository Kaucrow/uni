#include <iostream>
#include "lib/BST/bst.h"

using std::cout;

int main() {
    cout << "Begin execution\n";
    BST<int> bst(10);
    bst.insert(1);
    bst.insert(2);
    bst.insert(20);
    bst.insert(6);

    LinkedList<int> preorder = bst.get_preorder();

    for (auto val : preorder)
        cout << val << ' ';

    cout << '\n';

    cout << "End execution\n" << std::endl;
}