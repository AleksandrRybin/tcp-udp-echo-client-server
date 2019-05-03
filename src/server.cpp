#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <algorithm>

#include "digits.hpp"

const size_t MAX_MSG = 64000;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "usage: server <tcp_port> <udp_port>" << std::endl;
        return -1;
    }

    char buf[MAX_MSG];

    int tcp_port = atoi(argv[1]);

    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0) {
        std::cout << "Couldn't create tcp socket" << std::endl;
        return -1;
    }
    fcntl(tcp_socket, F_SETFL, O_NONBLOCK);

    sockaddr_in tcp_addr;
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_port = htons(tcp_port);
    tcp_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(tcp_socket, (sockaddr*) &tcp_addr, sizeof(tcp_addr)) < 0) {
        std::cout << "Couldn't bind tcp socket" << std::endl;
        close(tcp_socket);

        return -1;
    }

    if (listen(tcp_socket, 1) < 0) {
        std::cout << "Couldn't listen on tcp socket" << std::endl;
        close(tcp_socket);

        return -1;
    }

    std::cout << "tcp socket is listening on localhost:" << tcp_port << std::endl;

    int udp_port = atoi(argv[2]);

    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {
        std::cout << "Couldn't create udp socket" << std::endl;
        close(tcp_socket);

        return -1;
    }
    fcntl(udp_socket, F_SETFL, O_NONBLOCK);

    sockaddr_in udp_addr;
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_port = htons(udp_port);
    udp_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(udp_socket, (sockaddr*) &udp_addr, sizeof(udp_addr)) < 0) {
        std::cout << "Couldn't bind udp socket" << std::endl;

        close(tcp_socket);
        close(udp_socket);

        return -1;
    }

    while(true) {
        int tcp_client = accept4(tcp_socket, NULL, NULL, SOCK_NONBLOCK);

        if (tcp_client < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cout << "Couldn't accept on tcp socket" << std::endl;
        }
        else if (tcp_client < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            std::cout << "No connections on tcp socket" << std::endl;
            
            sockaddr_in udp_from_addr;
            socklen_t len = sizeof(udp_from_addr);

            int num_bytes_read = recvfrom(udp_socket, buf, MAX_MSG, 0, (sockaddr*) &udp_from_addr, &len);

            if (num_bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                std::cout << "Error reading from udp socket" << errno << std::endl;
                close(tcp_client);
                close(tcp_socket);
                
                return -1;
            }
            else if (num_bytes_read < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                std::cout << "No message found on udp socket" << std::endl;
                sleep(3);
            }
            else {
                std::cout << "Received by udp " << num_bytes_read << " bytes: " << buf << std::endl;

                Digits digits = Digits::find_digits(buf, num_bytes_read);
                digits.sort(false);
                digits.print();

                if (sendto(udp_socket, buf, num_bytes_read, 0, (sockaddr*) &udp_from_addr, len) < 0) {
                    std::cout << "Error sending to udp socket" << errno << std::endl;
                    close(tcp_client);
                    close(tcp_socket);
                
                    return -1;
                }
            }
        }
        else {
            while(true) {
                int num_bytes_read = recv(tcp_client, buf, MAX_MSG, 0);

                if (!num_bytes_read) {
                    break;
                }
                else if (num_bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                    std::cout << "Error reading from tcp socket" << errno << std::endl;
                    close(tcp_client);
                    close(tcp_socket);
                    
                    return -1;
                }
                else if (num_bytes_read < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    std::cout << "No message found" << std::endl;
                    sleep(3);
                }
                else {
                    std::cout << "Received by tcp " << num_bytes_read << " bytes: " << buf << std::endl;

                    Digits digits = Digits::find_digits(buf, num_bytes_read);
                    digits.sort(false);
                    digits.print();

                    if (send(tcp_client, buf, num_bytes_read, 0) < 0) {
                        std::cout << "Error sending to tcp socket" << std::endl;
                        close(tcp_client);
                        close(tcp_socket);
                    
                        return -1;
                    }
                }
            }

            close(tcp_client);
        }
    }

    if (close(tcp_socket) < 0) {
        std::cout << "Error close tcp socket" << std::endl;
        return -1;
    }

    return 0;
}