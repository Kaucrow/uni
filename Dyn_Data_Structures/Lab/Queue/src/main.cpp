#include <iostream>
#include "./lib/queue/queue.h"

using std::cout;

int main() {
    Queue<int> my_queue;
    my_queue.push(1);
    my_queue.push(4);
    my_queue.push(2);
    my_queue.push(3);
    my_queue.sort();
    for(int i = my_queue.len(); i > 0; i--) {
        cout << my_queue.pop() << '\n';
    }
}