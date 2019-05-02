#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <iostream>
#include <cstdlib>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "usage: client <tcp|udp> <port>" << std::endl;

        return -1;
    }

    int port = atoi(argv[2]);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int tcp_socket = 0;
    int udp_socket = 0;

    if (strcmp(argv[1], "tcp") == 0) {
        tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (tcp_socket < 0) {
            std::cout << "Couldn't create tcp socket" << std::endl;
            return -1;
        }

        if (connect(tcp_socket, (sockaddr*) &addr, sizeof(addr)) < 0) {
            std::cout << "Couldn't connect to tcp socket" << std::endl;
            close(tcp_socket);
            return -1;
        }
        else {
            std::cout << "Connected to tcp socket on localhost:" << port << std::endl;
        }
    }
    else if (strcmp(argv[1], "udp") == 0) {
        udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_socket < 0) {
            std::cout << "Couldn't create udp socket" << std::endl;
            return -1;
        }
    }
    else {
        std::cout << "invalid protocol" << std::endl;
        return -1;
    }
    
    std::string msg;
    msg.reserve(64000);

    char buf[64000];

    while(true){
        std::cout << "input message:" << std::endl;
        std::getline(std::cin, msg);

        if (msg == "quit") {
            break;
        }

        if (strcmp(argv[1], "tcp") == 0) {
            send(tcp_socket, msg.c_str(), msg.size(), 0);

            int num_bytes_read = recv(tcp_socket, buf, msg.size(), 0);

            if (num_bytes_read < 0) {
                std::cout << "Error reading from tcp socket" << std::endl;
                close(tcp_socket);
                
                return -1;
            }        
        }
        else if (strcmp(argv[1], "udp") == 0) {
            sendto(udp_socket, msg.c_str(), msg.size(), 0, (sockaddr*) &addr, sizeof(addr));

            int num_bytes_read = recvfrom(udp_socket, buf, msg.size(), 0, NULL, NULL);

            if (num_bytes_read < 0) {
                std::cout << "Error reading from udp socket" << std::endl;
                close(udp_socket);
                
                return -1;
            }        
        }

        std::cout << "from server: " << buf << std::endl;
    }
        
    if (tcp_socket) {
        close(tcp_socket);
    }

    if (udp_socket) {
        close(udp_socket);
    }

    return 0;
}