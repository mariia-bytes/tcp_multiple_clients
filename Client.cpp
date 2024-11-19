/*******************************************************************************
 * In this Client.cpp file class Client impelemted. 
 * The Client class declared in Client.h.
*******************************************************************************/

#include "Client.h"

extern const int buffer_size;

Client::Client(const std::string& server_ip, const unsigned int& port)
    : server_ip(server_ip), port(port), client_fd(-1) {
    client_fd = create_socket();
    connect_to_server();
}

Client::~Client() {
    if (client_fd != -1) {
        close(client_fd);
    }
}

int Client::create_socket() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void Client::connect_to_server() {
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
}

void Client::send_message() {
    char buffer[buffer_size] = {0};
    while (true) {
        std::cout << "> ";
        std::cin.getline(buffer, sizeof(buffer));

        if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
            perror("Send message failed");
        }
    }
}

void Client::receive_message() {
    char buffer[buffer_size] = {0};
    int valread;
    while (true) {
        valread = read(client_fd, buffer, buffer_size);
        if (valread > 0) {
            buffer[valread] = '\0';
            std::cout << "SERVER> " << buffer << std::endl;
        } else if (valread == 0) {
            std::cout << "Server closed the connection" << std::endl;
        } else {
            perror("Receive message failed");
        }
    }
}