#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE_SIZE 81  // Define maximum size for the queue

class Queue {
public:
    int front, rear, size;     // Indices for front and rear, size to track elements
    int data[MAX_QUEUE_SIZE]; // Static array to store provinces

    Queue();                  // Constructor to initialize the queue

    // Adds a province to the end of the queue
    void enqueue(int province);

    // Removes and returns the front province from the queue
    int dequeue();

    // Returns the front province without removing it
    int peek() const;

    // Checks if the queue is empty
    bool isEmpty() const;

    // Adds a priority province to the front of the queue
    void enqueuePriority(int province);

    // Clears the queue
    void clear();

    // Returns the current size of the queue
    int getSize() const;

private:
    // Utility function to advance indices in circular fashion
    int advance(int index) const;
};

#endif // QUEUE_H
