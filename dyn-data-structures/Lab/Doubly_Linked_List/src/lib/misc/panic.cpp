#include <stdexcept>
#include "panic.h"

void panic() {
    throw std::runtime_error("[ ERR ] Thread panicked.");
}

void panic(string msg) {
    throw std::runtime_error(msg);
}