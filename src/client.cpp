#include <string.h>

#include <iostream>
#include <string>
#include <memory>

#include "sockets.hpp"

#define CLIENT_EXIT "quit" // message to finish client program

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "usage: client <tcp|udp> <port>" << std::endl;
        return -1;
    }

    int port = atoi(argv[2]);

    if (port <= 0) {
        std::cout << "invalid port" << std::endl;
        return -1;
    }

    std::unique_ptr<INETSocket> socket;

    if (strcmp(argv[1], "tcp") == 0) {
        try {
            socket.reset(new TCPSocket(port, false));
        }
        catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl 
            << "errno: " << errno << std::endl;
            return -1;
        }
    }
    else if (strcmp(argv[1], "udp") == 0) {
        try {
            socket.reset(new UDPSocket(port, false));
        }
        catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl 
            << "errno: " << errno << std::endl;
            return -1;
        }
    }
    else {
        std::cout << "invalid protocol" << std::endl;
        return -1;
    }
    
    std::string msg;
    msg.reserve(MAX_MSG);

    while(true){
        std::cout << "input message:" << std::endl;
        std::getline(std::cin, msg);

        // check if get CLIENT_EXIT message
        if (msg == CLIENT_EXIT) {
            std::cout << "bye!" << std::endl;
            break;
        }

        socket->transmit(msg.c_str(), msg.size());

        int num_bytes_read = socket->read();

        if (num_bytes_read > 0) {
            std::cout << "from server: " << socket->get_buf() << std::endl;

            // check if get EXIT_MSG message
            if (strcmp(socket->get_buf(), EXIT_MSG) == 0) {
                std::cout << "bye!" << std::endl;
                break;
            }
        }
    }

    return 0;
}