#include <iostream>
#include <cstddef>
#include "../../Stack/src/lib/stack/stack.h"

using std::cout;

template <typename T>
Stack<T> sort_stack(Stack<T> &input, Stack<T> &aux);

int main() {
    Stack<int> stack1, stack2, stack3;

    stack1.push(3);
    stack1.push(6);
    stack1.push(1);
    stack1.push(2);

    stack2.push(9);
    stack2.push(4);
    stack2.push(5);
    stack2.push(12);

    stack3.push(1);
    stack3.push(4);
    stack3.push(2);
    stack3.push(8);

    Stack<int>* stacks[3] = {&stack1, &stack2, &stack3};

    size_t stack_size = stacks[0]->len();

    cout << stack_size << '\n';

    sort_stack(stack1, stack2);
}

template <typename T>
Stack<T> sort_stack(Stack<T> &input, Stack<T> &aux) {
    cout << "Hello world\n";
}