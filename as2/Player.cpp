#include "Player.h"
#include "AsteroidDash.h"
#include <iostream>

// Constructor to initialize the player's spacecraft, position, and ammo
Player::Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo, int lives)
        : spacecraft_shape(shape), position_row(row), position_col(col), player_name(player_name),
          max_ammo(max_ammo), current_ammo(max_ammo), lives(lives) {
    // Additional initialization if necessary
}

// Move player left within the grid boundaries
void Player::move_left() {
    if (position_col > 0) {  // Prevent moving beyond the left grid boundary
        position_col--;
    }
}

// Move player right within the grid boundaries
void Player::move_right(int grid_width) {
    if (position_col + spacecraft_shape[0].size() < grid_width) {  // Prevent moving beyond the right boundary
        position_col++;
    }
}

// Move player up within the grid boundaries
void Player::move_up() {
    if (position_row > 0) {  // Prevent moving beyond the top boundary
        position_row--;
    }
}

// Move player down within the grid boundaries
void Player::move_down(int grid_height) {
    if (position_row + spacecraft_shape.size() < grid_height) {  // Prevent moving beyond the bottom boundary
        position_row++;
    }
}

// Function to shoot a projectile
void Player::shoot(std::vector<std::vector<int>>& space_grid) {
    // Calculate the starting position of the projectile
    int projectileRow = position_row + spacecraft_shape.size() / 2; // Center row of the spacecraft
    int projectileCol = position_col + 1;                   // One cell to the right of the spacecraft

    // Move the projectile until it exits the grid or collides with an object
    while (projectileCol < static_cast<int>(space_grid[0].size())) {
        // Check if the projectile hits an object (non-zero value in the grid)
        if (space_grid[projectileRow][projectileCol] != 0) {
            std::cout << "Projectile hit an object at (" << projectileRow << ", " << projectileCol << ")!" << std::endl;

            // "Destroy" the object by setting the cell to 0
            space_grid[projectileRow][projectileCol] = 0;

            return; // Stop further movement of the projectile
        }

        // Update grid for projectile movement
        if (projectileCol > 0) {
            space_grid[projectileRow][projectileCol - 1] = 0; // Clear the previous position
        }
        space_grid[projectileRow][projectileCol] = 2; // Mark the new position of the projectile (2 represents a projectile)

        // Move the projectile one step to the right
        ++projectileCol;
    }

    // Clear the final position after the projectile exits the grid
    if (projectileCol > 0 && projectileCol <= static_cast<int>(space_grid[0].size())) {
        space_grid[projectileRow][projectileCol - 1] = 0;
    }

    std::cout << "Projectile exited the grid." << std::endl;
}

// Reload ammo (assuming reloading mechanism)
void Player::reload_ammo(int amount) {
    current_ammo = std::min(current_ammo + amount, max_ammo);  // Ensure ammo does not exceed max capacity
}

// Method to handle collisions or tick-based events
void Player::tick_update(vector<vector<bool>>& space_grid) {
    // Example of handling collision with celestial objects in the same row and column
    // This could be expanded based on specific game mechanics
    for (int i = 0; i < spacecraft_shape.size(); i++) {
        for (int j = 0; j < spacecraft_shape[i].size(); j++) {
            if (spacecraft_shape[i][j] && space_grid[position_row + i][position_col + j]) {
                lives--;  // Lose a life if a collision occurs
                space_grid[position_row + i][position_col + j] = false;  // Clear the grid position
            }
        }
    }
}

