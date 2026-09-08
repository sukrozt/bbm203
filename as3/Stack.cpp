#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() : top(-1) {}

// Adds a province to the top of the stack
void Stack::push(int province) {
    if (top < MAX_SIZE - 1) {
        data[++top] = province;
    } else {
        std::cerr << "Error: Stack overflow. Cannot push to a full stack.\n";
    }
}

// Removes and returns the top province from the stack
int Stack::pop() {
    if (isEmpty()) {
        std::cerr << "Error: Stack underflow. Cannot pop from an empty stack.\n";
        return -1; // Return an invalid value to indicate failure
    }
    return data[top--];
}

// Returns the top province without removing it
int Stack::peek() const {
    if (isEmpty()) {
        std::cerr << "Error: Stack is empty. No top element to peek.\n";
        return -1; // Return an invalid value to indicate failure
    }
    return data[top];
}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    return top == -1;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    return top + 1;
}
