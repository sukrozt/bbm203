#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {
public:
    vector<Client> clients;
    vector<vector<string>> all_command_data_vector;
    int number_of_clients;
    int MAX_MESSAGE_CHAR = 20;
    Client *sender = nullptr;
    Client *receiver = nullptr;
    vector<Client*> client_vector;
    vector<Packet*> packet_vector;
    int command_number;
    Network();
    ~Network();

    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port,
                     const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename);
    stack<Packet*> chunk_message(string &senderID, string &receiverID, string &message ,const string &sender_port, const string &receiver_port);
    vector<string> command_line_vector; //command as line in a vector
    Client* find_sender_receiver(vector<Client> &vector, basic_string<char> &basicString);

};

#endif  // NETWORK_H
