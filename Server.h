/*****************************************************************
 * In this Server.h file Server class declared.
 * Member functions implimented in file Server.cpp.
*****************************************************************/

#ifndef SERVER_H
#define SERVER_H

#include "shared.h"

class Server {
    private:
        int port;
        int server_fd;        
        struct sockaddr_in address;
        fd_set readfds;
        std::vector<int> client_sockets;

        int create_socket();
        void bind_socket();
        void listen_socket();
        void accept_connection();
        void handle_client(int client_socket);
        void close_client(int client_socket, size_t index);
        void setup_fdset();
        void check_activity();        
    
    public:
        Server (int port);
        ~Server();
        void run();    
};

#endif // SERVER_H