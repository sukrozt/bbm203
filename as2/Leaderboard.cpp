#include <algorithm>
#include "Leaderboard.h"

// Read the stored leaderboard from the file, loading each entry into a linked list.
void Leaderboard::read_from_file(const string &filename) {
    fstream file(filename);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        unsigned long score;
        time_t lastPlayed;
        string playerName;

        if (iss >> score >> lastPlayed >> playerName) {
            LeaderboardEntry *entry = new LeaderboardEntry(score, lastPlayed, playerName);
            insert(entry);  // Insert entry in sorted order.
        }
    }
    file.close();
}

// Write the current leaderboard to a file, formatted as "<score> <timestamp> <player_name>"
void Leaderboard::write_to_file(const string &filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
        return;
    }

    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        file << current->score << " " << current->last_played << " " << current->player_name << "\n";
        current = current->next;
    }
    file.close();
}

// Print the leaderboard to STDOUT with the specified formatting
void Leaderboard::print_leaderboard() {
    std::cout << "Leaderboard\n-----------\n";
    LeaderboardEntry *current = head_leaderboard_entry;
    int rank = 1;

    while (current && rank <= MAX_LEADERBOARD_SIZE) {
        std::tm *tm = std::localtime(&current->last_played);
        std::cout << rank << ". " << current->player_name << " " << current->score
                  << " " << std::put_time(tm, "%H:%M:%S/%d.%m.%Y") << "\n";
        current = current->next;
        rank++;
    }
}

// Insert a new entry in the leaderboard in descending order of scores,
// keeping only the top 10 entries.
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // Check if the leaderboard is empty or new entry should be at the head
    if (!head_leaderboard_entry ||
        new_entry->score > head_leaderboard_entry->score ||
        (new_entry->score == head_leaderboard_entry->score && new_entry->last_played < head_leaderboard_entry->last_played)) {

        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        // Find insertion point considering both score and timestamp
        LeaderboardEntry *current = head_leaderboard_entry;
        while (current->next &&
               (current->next->score > new_entry->score ||
                (current->next->score == new_entry->score && current->next->last_played < new_entry->last_played))) {
            current = current->next;
        }

        new_entry->next = current->next;
        current->next = new_entry;
    }

    // Trim the leaderboard to only the top 10 entries
    LeaderboardEntry *current = head_leaderboard_entry;
    int count = 1;
    while (current && current->next && count < MAX_LEADERBOARD_SIZE) {
        current = current->next;
        count++;
    }

    // Delete entries beyond the 10th
    if (current && current->next) {
        LeaderboardEntry *to_delete = current->next;
        current->next = nullptr;
        while (to_delete) {
            LeaderboardEntry *next = to_delete->next;
            delete to_delete;
            to_delete = next;
        }
    }
}

bool Leaderboard::isHighScore(int score) const {
    // Check if we have fewer than 10 entries or if this score is higher than the lowest in the top ten
    return entries.size() < MAX_ENTRIES || score > entries.back().score;
}

void Leaderboard::addScore(const std::string &name, int score, std::time_t timestamp) {
    // Only add the score if it qualifies as a high score
    if (isHighScore(score)) {
        // Add the new score entry
        entries.push_back(LeaderboardEntry(score, timestamp, name));
        sort();  // Sort the leaderboard after adding new entry

        // Trim entries to top ten if there are more than 10
        if (entries.size() > MAX_ENTRIES) {
            entries.pop_back();
        }
    }
}

void Leaderboard::sort() {
    // Sort by score in descending order; if scores are equal, sort by ascending timestamp
    std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry &a, const LeaderboardEntry &b) {
        return (a.score > b.score) || (a.score == b.score && a.timestamp < b.timestamp);
    });
}



// Destructor to free dynamically allocated memory for leaderboard entries
Leaderboard::~Leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        LeaderboardEntry *next = current->next;
        delete current;
        current = next;
    }
}

