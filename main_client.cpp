#include "shared.h"
#include "Client.h"

int main(int argc, char* argv[]) { 
   if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <Server IP> <Port>" << std::endl;
        return 1;
    }

    std::string server_ip = argv[1];
    unsigned int port = std::stoi(argv[2]);

    Client client(server_ip, port);

    std::thread t1(&Client::send_message, &client);
    std::thread t2(&Client::receive_message, &client);

    t1.join();
    t2.join();

    return 0;
} 