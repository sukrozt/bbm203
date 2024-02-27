#include "Network.h"
#include "ApplicationLayerPacket.h"
#include <fstream>
#include <sstream>

Network::Network() {
}

//queue arkadan pushluyo önden siliyo stack önden pushluyo
//frameler stack olacak framelerin toplandığı yer queue

string removeCharacter(const std::string &input, char target) {
    string result;

    for (char ch: input) {
        if (ch != target) {
            result += ch;
        }
    }

    return result;
}

string splitWhitespaceAtEnd(const std::string &input) {
    size_t lastNonWhitespace = input.find_last_not_of(" \t\n\r\f\v");
    if (lastNonWhitespace != std::string::npos) {
        return input.substr(0, lastNonWhitespace + 1);
    }
    return "";
}

Client *Network::find_sender_receiver(vector<Client> &vector, basic_string<char> &basicString) {
    for (int i = 0; i < vector.size(); i++) {
        if (vector[i].client_id == basicString.c_str()) {
            vector[i].connection = true;
            return &vector[i];
        }
    }
    Client *emptyc = new Client();
    emptyc->connection = false;
    client_vector.push_back(emptyc);
    return emptyc;
}

stack<Packet *> Network::chunk_message(string &senderID, string &receiverID, string &message, const string &sender_port,
                                       const string &receiver_port) {
    stack<Packet *> packet_frame;
    message = message.substr(0, MAX_MESSAGE_CHAR); //<20 messagelarda hata verebilir
    Client *sender = find_sender_receiver(clients, senderID);
    Client *receiver = find_sender_receiver(clients, receiverID);

    Packet *app_packet = new ApplicationLayerPacket(0, sender->client_id, receiver->client_id, message);
    packet_frame.push(app_packet);

    Packet *tra_packet = new TransportLayerPacket(1, sender_port, receiver_port);
    packet_frame.push(tra_packet);

    Packet *net_packet = new NetworkLayerPacket(2, sender->client_ip, receiver->client_ip);
    packet_frame.push(net_packet);

    Packet *phy_packet = new PhysicalLayerPacket(3, sender->client_mac, receiver->client_mac);
    packet_frame.push(phy_packet);

    sender->outgoing_queue.push(packet_frame);
    return packet_frame;
}

vector<Client> Network::read_clients(const string &filename) {
    fstream client_file(filename);
    string client_dataline;
    vector<string> data_as_vector;
    string data;

    int number = 1;
    while (getline(client_file, client_dataline)) {
        stringstream client_data(client_dataline);
        while (client_data >> data) {
            data_as_vector.push_back(data);
        }
        if (data_as_vector.size() == 1) {
            //number_of_clients = stoi(data_as_vector[0]);
            data_as_vector.clear();
        }
        if (client_dataline.size() >= 3) {
            Client newClient(data_as_vector[0], data_as_vector[1], data_as_vector[2]);
            data_as_vector.clear();
            clients.push_back(newClient);
            newClient.setNumber(number);
            number++;
        } else if (!client_dataline.empty()) {
            //number_of_clients = stoi(data_as_vector[0]);
        }
    }
    return clients;
}   

        void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
            // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
            fstream routing_file(filename);
            string routing_line;
            string receiverID;
            string nexthopID;
            unordered_map<string, string> routing_frame;
            while (getline(routing_file, routing_line)) {
                for (int i = 0 ; i<clients.size(); i++) {
                    if (routing_line.find('-')) {
                        clients[i].routing_table = routing_frame;
                        break;
                    } else {
                        string receiver(1, routing_line[0]);
                        string nexthop(1, routing_line[2]);
                        routing_frame[receiver] = nexthop;
                    }
                }
            }
        }

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    // TODO: Read commands from the given input file and return them as a vector of strings.
    vector<string> commands;
    string line;
    std::ifstream commandsFile(filename);
    getline(commandsFile, line);
    while(getline(commandsFile, line)){
        commands.push_back(line);
    }
    return commands;
}
void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                               const string &sender_port, const string &receiver_port) {
    
}

Network::~Network(){
        int client_count = client_vector.size();
        for(int i = 0; i < client_count; i++){
            Client* temp = client_vector.at(i);
            delete temp;
        }
        client_vector.clear();
}