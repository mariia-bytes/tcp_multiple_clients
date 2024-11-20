# TCP Multiple Clients

This project implements a TCP client-server relationship, enabling multiple clients to connect to a single server concurrently. The server echoes back any messages it receives to the client that sent them. The project now includes both server-side and client-side implementations.

## Current Features

- **Server-Side Implementation**: 
  - The server supports multiple clients (approximately up to 5, depending on the system running the project).
  - Each client can send messages concurrently.
  - The server echoes back the received messages to the respective clients.

- **Client-Side Implementation**: 
  - Clients can connect to the server over a specified IP address and port.
  - Messages sent by clients are relayed back by the server.

## How to Run

To set up and run the project on your local machine/machines:

1. Clone the repository:
    '''bash
    git clone https://github.com/mariia-bytes/tcp_multiple_clients.git
    cd tcp_multiple_clients

1. Create a build directory and navigate into it:
   ```bash
   mkdir build
   cd build

2. Build the project using CMake:
    ```bash
    cmake ..
    make

3. Start the server:
    ```bash
    ./server <port>
    Replace <port> with the desired port number (recommended range: 10,000–60,000).

4. Start a client:
    ```bash
    ./client <ip_address> <port>
    Replace <ip_address> with the server's IP address (use 127.0.0.1 for localhost) and <port> with the server's port.

## Possible future improvements to the project include:

1. Transforming the project into a peer-to-peer chat network rather than a traditional client-server model (e.g. enabling the server to send messages to clients).
2. Improving scalability.
3. Ensuring cross-platform compatibility.
4. Integrating encryption.
5. Developing a graphical user interface.


---

*This project began on November 11th, 2024*
