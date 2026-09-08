#include "Queue.h"
#include <iostream>

// Constructor to initialize the queue
Queue::Queue() : front(0), rear(-1), size(0) {}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    if (size == MAX_QUEUE_SIZE) {
        std::cerr << "Error: Queue overflow. Cannot enqueue.\n";
        return;
    }
    rear = advance(rear);  // Move rear to the next position in circular fashion
    data[rear] = province;
    size++;
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    if (isEmpty()) {
        std::cerr << "Error: Queue underflow. Cannot dequeue.\n";
        return -1; // Return an invalid value to indicate failure
    }
    int frontValue = data[front];
    front = advance(front); // Move front to the next position in circular fashion
    size--;
    return frontValue;
}

// Returns the front province without removing it
int Queue::peek() const {
    if (isEmpty()) {
        std::cerr << "Error: Queue is empty. Cannot peek.\n";
        return -1;
    }
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    return size == 0;
}

// Adds a priority province to the front of the queue
void Queue::enqueuePriority(int province) {
    if (size == MAX_QUEUE_SIZE) {
        std::cerr << "Error: Queue overflow. Cannot enqueue priority element.\n";
        return;
    }

    for (int i = size; i > 0; --i) {
        data[(front + i) % MAX_QUEUE_SIZE] = data[(front + i - 1) % MAX_QUEUE_SIZE];
    }

    data[front] = province;
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    size++;
}


// Clears the queue
void Queue::clear() {
    front = 0;
    rear = -1;
    size = 0;
}

// Returns the current size of the queue
int Queue::getSize() const {
    return size;
}

// Advances an index in circular fashion
int Queue::advance(int index) const {
    return (index + 1) % MAX_QUEUE_SIZE;
}
