/**************************************************
 * In this Client.h file Client class declared.
 * Member function implimented in file Client.cpp.
**************************************************/

#ifndef CLIENT_H
#define CLIENT_H

#include "shared.h"

class Client {
    private:
        unsigned int port;
        std::string server_ip;
        int client_fd;
        struct sockaddr_in server_address;

        int create_socket();
        void connect_to_server();

    public:
        Client(const std::string& server_ip, const unsigned int& port);
        ~Client();

        void send_message();
        void receive_message();
};

#endif // CLIENT_H