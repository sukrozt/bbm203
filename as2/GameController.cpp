#include "GameController.h"
#include <fstream>
#include <iostream>

// Constructor to initialize the game instance
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name
) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name, player_name);
}


// Reads commands from the given input file and executes each command in a game tick
void GameController::play(const string &commands_file) {
    ifstream file(commands_file);
    string command;

    if (!file.is_open()) {
        cerr << "Error: Could not open commands file." << endl;
        return;
    }

    while (getline(file, command)) {
        // Interpret and execute each command
        if (command == "PRINT_GRID") {
            game->print_space_grid();
        } else if (command == "MOVE_UP") {
            game->player->move_up();
        } else if (command == "MOVE_DOWN") {
            game->player->move_down(game->getGridHeight());
        } else if (command == "MOVE_RIGHT") {
            game->player->move_right(game->getGridWidth());
        } else if (command == "MOVE_LEFT") {
            game->player->move_left();
        } else if (command == "SHOOT") {
            game->shoot();
        } else if (command == "NOP") {
            // No operation, do nothing
        } else {
            cerr << "Error: Unknown command '" << command << "'" << endl;
        }

        // Advance the game by one tick after each command
       game->update_space_grid();
    }
    game->gameOver();
    file.close();
}

// Destructor to clean up dynamically allocated memory
GameController::~GameController() {
    delete game;
}
