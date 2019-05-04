#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

#define MAX_MSG 64000
#define EXIT_MSG "server power off"
#define MAX_TCP_CLIENTS 5

class INETSocket {

public:

    INETSocket(int port, bool is_listening, const char* ip);
    virtual ~INETSocket();
    
    virtual int read() = 0;
    virtual int transmit(const char* msg, int len) = 0;

    const char* get_buf() const;

protected:
    
    sockaddr_in addr;
    int sock = 0;
    int port;
    char buf[MAX_MSG + 1] = {0};
    bool is_listening;
};

class TCPSocket : public INETSocket {

public:
    
    TCPSocket(int port, bool is_listening, const char* ip="127.0.0.1");
    
    virtual int read() override;
    virtual int transmit(const char* msg, int len) override;

private:
    bool accept();

    int client = 0;
};

class UDPSocket : public INETSocket {

public:

    UDPSocket(int port, bool is_listening, const char* ip="127.0.0.1");

    virtual int read() override;
    virtual int transmit(const char* msg, int len) override;

private:
   
    sockaddr_in client_addr;
    socklen_t client_addr_len;
};