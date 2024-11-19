/***********************************************
 * This shared.h file declares all headers used
 * in server.cpp and client.cpp
***********************************************/

#ifndef SHARED_H
#define SHARED_H

#include <iostream>  // for cout/cerr
#include <arpa/inet.h>  // for ip inet_pton()
#include <netinet/in.h> // for address
#include <sys/select.h> // for io multiplexing (select)
#include <sys/socket.h> // for socket
#include <unistd.h>  // for close()
#include <vector> // for storing client
#include <string>
#include <string.h> // for memset
#include <cstdio> // for perror
#include <algorithm>
#include <thread>

static const int buffer_size = 1024;

#endif // SHARED_H