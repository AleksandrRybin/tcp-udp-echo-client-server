#include "string.h"

#include <iostream>
#include <memory>

#include "numbers.hpp"
#include "sockets.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "usage: server <tcp_port> <udp_port>" << std::endl;
        return -1;
    }

    int tcp_port = atoi(argv[1]);
    int udp_port = atoi(argv[2]);

    if (tcp_port <= 0 || udp_port <= 0) {
        std::cout << "invalid port" << std::endl;
        return -1;
    }
    
    std::unique_ptr<INETSocket> tcp_socket;
    std::unique_ptr<INETSocket> udp_socket;

    try {
        tcp_socket.reset(new TCPSocket(tcp_port, true));
        udp_socket.reset(new UDPSocket(udp_port, true));
    }
    catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl 
        << "errno: " << errno << std::endl;
        return -1;
    }

    while(true) {
        int num_bytes_read = tcp_socket->read();

        if (num_bytes_read > 0){
            std::cout << "Received by tcp "
            << num_bytes_read << " bytes: " << tcp_socket->get_buf() << std::endl;

            Numbers numbers = Numbers::find_numbers(tcp_socket->get_buf(), num_bytes_read);
            numbers.sort(false);
            numbers.print();

            tcp_socket->transmit(tcp_socket->get_buf(), num_bytes_read);
        }

        num_bytes_read = udp_socket->read();

        if (num_bytes_read > 0) {
            std::cout << "Received by udp " 
            << num_bytes_read << " bytes: " << udp_socket->get_buf() << std::endl;

            Numbers numbers = Numbers::find_numbers(tcp_socket->get_buf(), num_bytes_read);
            numbers.sort(false);
            numbers.print();

            udp_socket->transmit(udp_socket->get_buf(), num_bytes_read);
        }

        if (strcmp(EXIT_MSG, tcp_socket->get_buf()) == 0 ||
            strcmp(EXIT_MSG, udp_socket->get_buf()) == 0) {
            std::cout << "bye!" << std::endl;
            break;
        }
    }

    return 0;
}