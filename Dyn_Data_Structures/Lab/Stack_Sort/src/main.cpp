#include <iostream>
#include <cstddef>
#include "../../../../Tools/Cpp/termcolor.hpp"
#include "../../Stack/src/lib/stack/stack.h"

#define color termcolor

using std::cout;

template <typename T>
void sort_stack(Stack<T> &input, Stack<T> &aux, bool display = false);

int main() {
    Stack<int> stacks[3];

    stacks[0].push(3);
    stacks[0].push(6);
    stacks[0].push(1);
    stacks[0].push(2);

    stacks[1].push(9);
    stacks[1].push(4);
    stacks[1].push(5);
    stacks[1].push(12);

    stacks[2].push(1);
    stacks[2].push(4);
    stacks[2].push(2);
    stacks[2].push(8);

    cout << "----- " << color::cyan << "SORTING STACK 0" << color::reset << " -----\n\n" << color::reset;
    sort_stack(stacks[0], stacks[2], true);
    cout << "----- " << color::cyan << "SORTING STACK 1" << color::reset << " -----\n\n" << color::reset;
    sort_stack(stacks[1], stacks[2], true);
    cout << "----- " << color::cyan << "SORTING STACK 0" << color::reset << " -----\n\n" << color::reset;
    sort_stack(stacks[2], stacks[0], true);
}

template <typename T>
void sort_stack(Stack<T> &input, Stack<T> &aux, bool display) {
    if(display)
        cout << "-> " << color::yellow << "Unsorted input: " << input.to_string() << "\n\n" << color::reset;

    size_t input_size = input.len();
    size_t iterations = 0;

    while(input.len() > 0) {
        iterations++;
        T tmp = input.pop();

        while(aux.len() > input_size && aux.peek() > tmp)
            input.push(aux.pop());

        aux.push(tmp);

        if(display)
            cout    << "Element taken out: " << tmp << '\n'
                    << "Input: " << input.to_string() << '\n'
                    << "Aux: " << aux.to_string() << "\n\n";
    }
    
    while(aux.len() > input_size) {
        input.push(aux.pop());
    }
    
    if(display)
        cout << "-> " << color::green << "Done.\n" << color::reset
             << color::bold << "   Sorted in " << iterations << " iterations.\n" << color::reset
             << color::yellow << "   Sorted input: " << input.to_string() << "\n\n" << color::reset;
}