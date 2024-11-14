/*********************************************************
 * This main_server.cpp file runs the server based on
 * Server class declared in Server.h and implimented in
 * Server.cpp.
 * 
 * The main function takes as an argument a port number
 * to run the server on.
*********************************************************/

#include "shared.h"
#include "Server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <Port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    Server server(port);
    server.run();
    
    return 0;
}