#include "CelestialObject.h"


// Constructor to initialize CelestialObject with essential properties
// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape),
          object_type(type),
          starting_row(start_row),
          time_of_appearance(time_of_appearance) {

    // Calculate the height and width of the shape based on the provided 2D vector.
    height = shape.size();            // Number of rows in the shape.
    width = (height > 0) ? shape[0].size() : 0;  // Number of columns in the first row if height > 0.

    // Initialize the rotation pointers to null since this is the base rotation.
    right_rotation = nullptr;
    left_rotation = nullptr;
    next_celestial_object = nullptr;
}



// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
    const CelestialObject *current = other->right_rotation;
    CelestialObject *last_rotation = this;

    while (current && current != other) {
        // Create a new CelestialObject for the current rotation
        auto *rotated_object = new CelestialObject(current->shape, current->object_type, current->starting_row,
                                                              current->time_of_appearance);

        // Link the last copied rotation to this new rotation
        last_rotation->right_rotation = rotated_object;
        rotated_object->left_rotation = last_rotation;

        // Move to the new rotation
        last_rotation = rotated_object;
        current = current->right_rotation;
    }

    // Complete the circular link by linking the last rotation back to this object
    last_rotation->right_rotation = this;
    this->left_rotation = last_rotation;
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    // TODO: Your code here
}