#include "AsteroidDash.h"


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

int AsteroidDash::getGridHeight() {
    return space_grid.size();
}

int AsteroidDash::getGridWidth() {
    return space_grid[0].size();
}

Player *AsteroidDash::getPlayer() {
    return player;
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    std::ifstream file(input_file);
    space_grid.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::istringstream iss(line);
        int cell;
        while (iss >> cell) {
            row.push_back(cell);
        }
        space_grid.push_back(row);
    }
    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    ifstream file(player_file_name);
    string line;
    int position_row = 0, position_col = 0;
    vector<vector<bool>> shape;

    // Read the initial position from the first line
    if (getline(file, line)) {
        istringstream pos_stream(line);
        pos_stream >> position_row >> position_col;
    }

    // Read the spacecraft shape line by line
    while (getline(file, line)) {
        istringstream shape_stream(line);
        vector<bool> shape_row;
        int value;

        while (shape_stream >> value) {
            shape_row.push_back(value == 1);  // Convert 1 to true, 0 to false
        }

        shape.push_back(shape_row);
    }

    // Initialize the Player instance with the provided shape, position, and name
    player = new Player(shape, position_row, position_col, player_name);
    file.close();
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) { /*
    ifstream file(input_file);
    CelestialObject *tail = nullptr;
    string line;
    char open_char;
    char close_char;
    vector<vector<bool>> shape;
    vector<bool> row;
    ObjectType type;
    int start_row = 0, time_of_appearance = 0;
    while (getline(file, line)) {
        if (line.empty() && !shape.empty()) {
            CelestialObject *new_celestial_object = new CelestialObject(shape, type, start_row, time_of_appearance);
            create_rotate_linkedlist(new_celestial_object);
            shape.clear();
            if (!celestial_objects_list_head) {
                celestial_objects_list_head = new_celestial_object;
            } else {
                tail->next_celestial_object = new_celestial_object;
            }
            tail = new_celestial_object;
        }
        if(!line.empty()){
            for (char c: line) {
                if (c == '[' || c == '{') {
                    open_char = c;
                    close_char = (open_char == '[') ? ']' : '}';
                    if (open_char == '[') type = ASTEROID;
                } else if (c == close_char) {
                    if (line[0] == open_char) {
                        shape.push_back(row);
                        row.clear();
                    }
                } else if (c == 's') {
                    size_t pos = line.find(':');
                    if (pos != std::string::npos) { // Check if ':' was found
                        start_row = std::stoi(line.substr(pos + 1));
                    }
                    break;
                } else if (c == 't') {
                    size_t pos = line.find(':');
                    if (pos != std::string::npos) { // Check if ':' was found
                        time_of_appearance = std::stoi(line.substr(pos + 1));
                    }
                    break;
                } else if (c == 'e') {
                    size_t pos = line.find(':');
                    if (pos != std::string::npos) { // Check if ':' was found
                        if (line.substr(pos + 1) == "life") type = LIFE_UP;
                        else if (line.substr(pos + 1) == "ammo") type = AMMO;
                    }
                    break;
                } else if (c == '1' || c == '0') {
                    row.push_back(c == '1');
                    if (line[0] == open_char && line[line.length() - 1] == close_char) {
                        if (row.size() == line.length() - 2) {
                            shape.push_back(row);
                            row.clear();
                        }
                    } else if (line[0] == open_char) {
                        if (row.size() == line.length() - 1) {
                            shape.push_back(row);
                            row.clear();
                        }
                    } else if (line[line.length() - 1] == close_char) {
                        if (row.size() == line.length() - 1) {
                            shape.push_back(row);
                            row.clear();
                        }
                    } else {
                        if (row.size() == line.length()) {
                            shape.push_back(row);
                            row.clear();
                        }

                    }
                }
            }
        }

    }*/
}

// Rotates a 2D matrix 90 degrees clockwise.
vector<vector<bool>> rotateShapeClockwise(const vector<vector<bool>> &shape) {
    // Define the rotated matrix with transposed dimensions
    std::vector<std::vector<bool>> rotated(shape[0].size(), std::vector<bool>(shape.size(), false));

    // Rotate the matrix
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[0].size(); ++j) {
            rotated[j][shape.size() - 1 - i] = shape[i][j];
        }
    }

    // Debug: Print the rotated matrix
    cout << "Rotated Matrix:" << endl;
    for (const auto &row: rotated) {
        for (bool cell: row) {
            cout << (cell ? '1' : '0') << " ";
        }
        cout << endl;
    }

    return rotated;
}

// Function to create right rotations and link them in a circular doubly linked list.
void AsteroidDash::create_rotate_linkedlist(CelestialObject *celestialObject) {
    // Check if the object is a 1x1 matrix; if so, skip rotation setup
    if (celestialObject->shape.size() == 1 && celestialObject->shape[0].size() == 1) {
        celestialObject->right_rotation = celestialObject;
        celestialObject->left_rotation = celestialObject;
        return;
    }

    // Start the rotation setup for objects larger than 1x1
    CelestialObject *current = new CelestialObject(celestialObject);
    for (size_t i = 0; i < 3; ++i) {  // Create three rotations to complete the circular linked list
        CelestialObject *rotated_object = new CelestialObject(current->shape, current->object_type,
                                                              current->starting_row,
                                                              current->time_of_appearance);
        rotated_object->shape = rotateShapeClockwise(current->shape);

        // Link rotations
        current->right_rotation = rotated_object;
        rotated_object->left_rotation = current;
        rotated_object->next_celestial_object = current->next_celestial_object;  // Link to next celestial object
        current = rotated_object;

    }

    // Complete the circular link by linking the last rotation back to the original object
    celestialObject->left_rotation = current;
    current->right_rotation = celestialObject;
}


// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    for (const auto& row : space_grid) {
        for (int cell : row) {
            if (cell == 0)
                std::cout << unoccupiedCellChar;
            else if (cell == 1)
                std::cout << occupiedCellChar; // Asteroid
            else if (cell == 2)
                std::cout << "→"; // Projectile
            else
                std::cout << "?"; // Unknown element
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    // Step 1: Reset the grid to default (unoccupied state)
    for (auto &row : space_grid) {
        std::fill(row.begin(), row.end(), 0); // Assuming 0 is the default empty value
    }

    // Step 2: Place celestial objects on the grid
    CelestialObject *current = celestial_objects_list_head;
    while (current) {
        int startRow = current->starting_row;
        int startCol = 0;  // Assuming CelestialObject has starting_column or similar

        // Get the current shape to place on the grid
        const vector<vector<bool>>& shape = current->shape;

        for (int i = 0; i < shape.size(); ++i) {
            for (int j = 0; j < shape[i].size(); ++j) {
                if (shape[i][j]) {  // Only set grid cells for true values in the shape
                    int gridRow = startRow + i;
                    int gridCol = startCol + j;

                    // Check if within grid bounds
                    if (gridRow >= 0 && gridRow < getGridHeight() && gridCol >= 0 && gridCol < getGridWidth()) {
                        space_grid[gridRow][gridCol] = 1;  // Assuming 1 denotes a celestial object
                    }
                }
            }
        }

        current = current->next_celestial_object; // Move to the next celestial object in the linked list
    }

    // Step 3: Place player on the grid
    int playerRow = player->position_row;
    int playerCol = player->position_col;
    const vector<vector<bool>>& playerShape = player->spacecraft_shape;

    for (int i = 0; i < playerShape.size(); ++i) {
        for (int j = 0; j < playerShape[i].size(); ++j) {
            if (playerShape[i][j]) {  // Only set grid cells for true values in the shape
                int gridRow = playerRow + i;
                int gridCol = playerCol + j;

                // Check if within grid bounds
                if (gridRow >= 0 && gridRow < getGridHeight() && gridCol >= 0 && gridCol < getGridWidth()) {
                    space_grid[gridRow][gridCol] = 2;  // Assuming 2 denotes the player
                }
            }
        }
    }

    // Optional: Step 4 - Handle collisions (e.g., player and celestial objects overlap)
    // Here, you would check for cells that contain both player and celestial object identifiers and process accordingly.
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo.
void AsteroidDash::shoot() {
    if (!player) {
        std::cerr << "Player not initialized." << std::endl;
        return;
    }

    // Check if the player has enough ammo to shoot
    if (player->current_ammo > 0) {
        // Decrease player's ammo and launch the projectile
        player->shoot(space_grid);  // space_grid is assumed to be the game's main grid

        std::cout << "Projectile launched! Ammo remaining: " << player->current_ammo << std::endl;
    } else {
        std::cerr << "Not enough ammo to shoot." << std::endl;
    }
}
// Update score when a projectile hits an asteroid cell
void AsteroidDash::hitAsteroidCell() {
    current_score += 10; // 10 points for hitting a cell
    std::cout << "Score updated: " << current_score << " (Hit Asteroid Cell)" << std::endl;
}

// Update score when an entire asteroid is destroyed
void AsteroidDash::destroyAsteroid(int asteroidOriginalSize) {
    int bonus = 100 * asteroidOriginalSize; // 100 points per cell of original asteroid size
    current_score += bonus;
    std::cout << "Score updated: " << current_score << " (Destroyed Asteroid)" << std::endl;
}

// Add survival points for each game tick
void AsteroidDash::surviveTick() {
    current_score += 1; // 1 point for each tick survived
    std::cout << "Score updated: " << current_score << " (Survived Tick)" << std::endl;
}

// End of game handling and updating leaderboard if necessary

void AsteroidDash::endGame() {
    // Print the termination reason
    std::cout << "GAME OVER!" << "\n";
    //std::cout << termination_reason << "\n";

    // Print the current tick
    //std::cout << "Tick: " << current_tick << "\n";

    // Print the player's remaining lives, ammo, and score
    std::cout << "Lives: " << player->lives << "\n";
    std::cout << "Ammo: " << player->current_ammo << "\n";
    //std::cout << "Score: " << score << "\n";

    // Check and display the high score
    unsigned long high_score = (leaderboard.head_leaderboard_entry)
                               ? leaderboard.head_leaderboard_entry->score
                               : 0;
    //std::cout << "High Score: " << std::max(high_score, score) << "\n";

    // Display the player's name
    std::cout << "Player: " << player->player_name << "\n";

    // Print the final state of the space grid
    print_space_grid();

    // Update the leaderboard if the player's score qualifies
    /*if (score > high_score || !leaderboard.head_leaderboard_entry || leaderboard.head_leaderboard_entry->score < score) {
        LeaderboardEntry *new_entry = new LeaderboardEntry(score, std::time(nullptr), player->player_name);
        leaderboard.insert(new_entry);
    }*/

    // Save the updated leaderboard to file
    leaderboard.write_to_file(leaderboard_file_name);
}

// Example function that calls endGame when game finishes
void AsteroidDash::gameOver() {
    endGame();
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    CelestialObject *current = celestial_objects_list_head;
    while (current) {
        CelestialObject *rotation = current->right_rotation;
        while (rotation && rotation != current) {
            CelestialObject *nextRotation = rotation->right_rotation;
            delete rotation;
            rotation = nextRotation;
        }
        CelestialObject *nextBlock = current->next_celestial_object;
        delete current;
        current = nextBlock;
    }
    celestial_objects_list_head = nullptr;
}





