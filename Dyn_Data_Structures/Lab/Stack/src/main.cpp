#include <iostream>
#include "lib/stack/stack.h"

using std::cout;

int main() {
    Stack<int> my_stack;
    my_stack.push(9);
    my_stack.push(3);
    cout << my_stack.pop() << '\n';
    cout << my_stack.pop() << '\n';
    my_stack.push(7);
    cout << my_stack.peek() << '\n';
    cout << my_stack.peek() << '\n';
}