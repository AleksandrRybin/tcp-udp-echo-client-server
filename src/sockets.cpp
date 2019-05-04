#include "arpa/inet.h"

#include "sockets.hpp"

INETSocket::INETSocket(int port, bool is_listening, const char* ip) {
    this->port = port;
    this->is_listening = is_listening;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (!inet_pton(AF_INET, ip, &addr.sin_addr)) {
        throw std::runtime_error("invalid ip");
    }
}

INETSocket::~INETSocket() {
    if (sock) {
        close(sock);
    }
}

const char* INETSocket::get_buf() const {
    return buf;
}

TCPSocket::TCPSocket(int port, bool is_listening, const char* ip) 
: INETSocket(port, is_listening, ip)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        throw std::runtime_error("Couldn't create tcp socket");
    }

    if (is_listening) {
        if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
            close(sock);
            throw std::runtime_error("Couldn't set O_NONBLOCK mode to tcp socket");
        }
    }

    if (is_listening) {
        if (bind(sock, (sockaddr*) &addr, sizeof(addr)) < 0) {
            close(sock);
            throw std::runtime_error("Couldn't bind tcp socket");
        }

        if (listen(sock, MAX_TCP_CLIENTS) < 0) {
            close(sock);
            throw std::runtime_error("Couldn't listen on tcp socket");
        }

        std::cout << "tcp socket is listening on "
        << inet_ntoa(addr.sin_addr) << ":" << port << std::endl;
    }
    else {
        if (connect(sock, (sockaddr*) &addr, sizeof(addr)) < 0) {
            close(sock);
            throw std::runtime_error("Couldn't connect to tcp socket");
        }

        std::cout << "Connected to tcp socket on " 
        << inet_ntoa(addr.sin_addr) << ":" << port << std::endl;
    }
}

TCPSocket::~TCPSocket() {
    if (client) {
        close(client);
    }
}

bool TCPSocket::accept() {
    client = accept4(sock, NULL, NULL, SOCK_NONBLOCK);

    return client > 0;
}

int TCPSocket::read() {
    int num_bytes_read;

    if (is_listening) {
        if (client > 0) {
            num_bytes_read = recv(client, buf, MAX_MSG, 0);
        }
        else if (accept()) {
            num_bytes_read = recv(client, buf, MAX_MSG, 0);
        }
        else {
            num_bytes_read = -1;
        }
    
        if (num_bytes_read == 0) {
            close(client);
            client = 0;
        }
    }
    else {
        num_bytes_read = recv(sock, buf, MAX_MSG, 0);
    }

    if (num_bytes_read > 0 && num_bytes_read < MAX_MSG) {
        buf[num_bytes_read] = '\0';
    }

    return num_bytes_read;
}

int TCPSocket::transmit(const char* msg, int len) {
    if (is_listening) {
        return send(client, msg, len, 0);
    }
    else {
        return send(sock, msg, len, 0);
    }
}

UDPSocket::UDPSocket(int port, bool is_listening, const char* ip)
: INETSocket(port, is_listening, ip)
{
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        throw std::runtime_error("Couldn't create udp socket");
    }

    if (is_listening) {
        if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
            close(sock);
            throw std::runtime_error("Couldn't set O_NONBLOCK mode to udp socket");
        }

        if (bind(sock, (sockaddr*) &addr, sizeof(addr)) < 0) {
            close(sock);
            throw std::runtime_error("Couldn't bind udp socket");
        }

        client_addr_len = sizeof(client_addr);

        std::cout << "udp socket is listening on "
        << inet_ntoa(addr.sin_addr) << ":" << port << std::endl;
    }
}

int UDPSocket::read() {
    int num_bytes_read;

    if (is_listening) {
        num_bytes_read = recvfrom(sock, buf, MAX_MSG, 0, (sockaddr*) &client_addr, &client_addr_len);
    }
    else {
        num_bytes_read = recvfrom(sock, buf, MAX_MSG, 0, NULL, NULL);
    }

    if (num_bytes_read > 0 && num_bytes_read < MAX_MSG) {
        buf[num_bytes_read] = '\0';
    }

    return num_bytes_read;
}

int UDPSocket::transmit(const char* msg, int len) {
    if (is_listening) {
        return sendto(sock, msg, len, 0, (sockaddr*) &client_addr, client_addr_len);
    }
    else {
        return sendto(sock, msg, len, 0, (sockaddr*) &addr, sizeof(addr));
    }
}