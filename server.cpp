#include <iostream>  // for cout/cerr
#include <arpa/inet.h>  // for ip inet_pton()
#include <netinet/in.h> // for address
#include <sys/select.h> // for io multiplexing (select)
#include <sys/socket.h> // for socket
#include <unistd.h>  // for close()
#include <vector> // for storing client
#include <string.h> // for memset


struct client_details {
    int32_t client_fd; // client file descriptor
    int32_t server_fd; // server file descriptor
    std::vector<int> client_list; // for storing all the client fd

    client_details(void) { // very bizzare way to declare but we'll see
        this->client_fd = -1;
        this->server_fd = -1;
    }
};


// notice: I highly disagree with hardcoding those numbers
// but I'll do it more gracefully later
const int port = 40277;
const char ip[] = "127.0.0.1";
const int backlog = 5; // max number of connection allowed


int main() {
    auto client = new client_details();
    client->server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client->server_fd == -1) {
        std::cerr << "Socket creation error" << std::endl;
        delete client;
        exit(1);
    } else {
        std::cout << "Socket created successfully" << std::endl;
    }

    // setting server_fd to allow multiple connection
    int opt = 1;
    if (setsockopt(client->server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof opt) < 0) {
        std::cerr << "setsocketopt error" << std::endl; // what is setsocketopt???
        delete client;
        exit(2);
    }

    // setting the server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    // binding the server address
    if (bind(client->server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind error" << std::endl;
        delete client;
        exit(3);
    } else {
        std::cout << "Server's listening" << std::endl;
    }

     // start listening on the server socket
    if (listen(client->server_fd, backlog) < 0) {
        std::cerr << "Listen error" << std::endl;
        delete client;
        exit(4);
    }

    fd_set read_fds;
    size_t val_read;
    int max_fd;
    int sd = 0;
    int activity;

    while (true) {
        std::cout << "Waiting for activity..." << std::endl;
        FD_ZERO(&read_fds);
        FD_SET(client->server_fd, &read_fds);
        max_fd = client->server_fd;

        // copy the clients list to read_fds to listen all clients
        for (auto sd:client->client_list) {
            FD_SET(sd, &read_fds);
            if (sd > max_fd) {
                max_fd = sd;
            }
        }
        
        // this if statement is weird
        if (sd > max_fd) {
            max_fd = sd;
        }

        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            std::cerr << "Select error" << std::endl;
            continue;
        }

        // if something happened on the server_fd 
        // then it's an incoming connection
        if (FD_ISSET(client->server_fd, &read_fds)) {
            int new_socket = accept(client->server_fd, (struct sockaddr*)NULL, NULL);
            if (new_socket < 0) {
                std::cerr << "Accept error" << std::endl;
                continue;
            }
        client->client_list.push_back(new_socket);
        std::cout << "\nNew client connected" << std::endl;
        std::cout << "Socket FD: " << new_socket << std::endl;
        std::cout << "IP: " << inet_ntoa(server_addr.sin_addr) << std::endl;
        std::cout << "Port: " << ntohs(server_addr.sin_port) << std::endl;
        }

        char buffer[4096];
        for (size_t i = 0; i < client->client_list.size(); i++) {
            sd = client->client_list[i];
            if (FD_ISSET(sd, &read_fds)) {
                memset(buffer, 0, sizeof(buffer));
                val_read = read(sd, buffer, sizeof(buffer) - 1);
                if (val_read <= 0) {
                    std::cout << "\nClient disconnected" << std::endl;
                    close(sd);
                    client->client_list.erase(client->client_list.begin() + i);
                    i--;
                } else {
                    buffer[val_read] = '\0';
                    std::cout << "CLIENT> " << buffer << std::endl;
                }
            }
        }
    }
    delete client;
    return 0;
}