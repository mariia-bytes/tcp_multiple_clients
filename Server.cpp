/*****************************************************************************
 * In this Server.cpp implimented all member functions of the class
 * Server declared in Server.h
******************************************************************************/

#include "Server.h"

extern const int buffer_size;

Server::Server(const unsigned int& port)
    : port(port), server_fd(-1) {
    server_fd = create_socket();
    bind_socket();
    listen_socket();
    client_sockets.reserve(FD_SETSIZE);
}

Server::~Server() {
    if (server_fd != -1) {
        close(server_fd);
    }

    for (int client_socket : client_sockets) {
        if (client_socket != -1) {
            close(client_socket);
        }
    }
}

int Server::create_socket() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    return fd;
}

void Server::bind_socket() {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

void Server::listen_socket() {
    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
}

void Server::accept_connection() {
    int new_socket;
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);

    if ((new_socket = accept(server_fd, (struct sockaddr*)&client_address, &address_length)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    client_sockets.push_back(new_socket);
    std::cout << "\nNew connection (fd: " << new_socket 
              << ", ip: " << inet_ntoa(client_address.sin_addr) 
              << ", port: " << ntohs(client_address.sin_port) 
              << ")" << std::endl;
}

void Server::handle_client(int client_socket) {
    char buffer[buffer_size];
    int valread = read(client_socket, buffer, buffer_size);

    if (valread == 0) {
        auto it = std::find(client_sockets.begin(), client_sockets.end(), client_socket);
        if (it != client_sockets.end()) {
            close_client(client_socket, std::distance(client_sockets.begin(), it));
        }
    } else {
        // receive_message();
        buffer[valread] = '\0';
        std::cout << "CLIENT(" << client_socket << ")> " << buffer << std::endl;
        send(client_socket, buffer, valread, 0); // use this to echo message back to the client
    }
}

// the code below is bad but I need it for the future
/*
void Server::receive_message() {
    char buffer[buffer_size] = {0};
    int valread;
    while (true) {
        for (auto client_fd : client_sockets) {
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
}
*/

void Server::close_client(int client_socket, size_t index) {
    struct sockaddr_in addr;
    socklen_t address_length = sizeof(addr);

    getpeername(client_socket, (struct sockaddr*)&addr, &address_length);

    std::cout << "\nClient (" << client_socket 
              << ", " << inet_ntoa(addr.sin_addr) 
              << ", " << ntohs(addr.sin_port) 
              << ") disconnected" << std::endl;

    close(client_socket);

    client_sockets[index] = -1; // -1 indicates that the socket is closed
}

void Server::setup_fdset() {
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);

    for (int client_socket : client_sockets) {
        if (client_socket > 0) {
            FD_SET(client_socket, &readfds);
        }
    }
}

void Server::check_activity() {
    int max_sd = server_fd;

    for (int client_socket : client_sockets) {
        if (client_socket > 0 && client_socket > max_sd) {
            max_sd = client_socket;
        }
    }

    int activity = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);

    if (activity < 0 && errno != EINTR) {
        perror("Select error");
    }
}

void Server::run() {
    std::cout << "Waiting for connection ..." << std::endl;

    while (true) {
        setup_fdset();
        check_activity();

        if (FD_ISSET(server_fd, &readfds)) {
            accept_connection();
        }

        for (size_t i = 0; i < client_sockets.size(); i++) {
            int client_socket = client_sockets.at(i);
            if (FD_ISSET(client_socket, &readfds)) {
                handle_client(client_socket);
            }
        }
    }
}