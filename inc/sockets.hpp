#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

#define MAX_MSG 64000 // max size of message in bytes
#define EXIT_MSG "server power off" // message to finish server from client
#define MAX_TCP_CLIENTS 10 // max tcp clients to serve at one moment

class INETSocket {

public:

    // is_listening - if true socket in listening mode
    // else socket connects to another socket
    INETSocket(int port, bool is_listening, const char* ip);

    virtual ~INETSocket();
    
    // read data from current client to internal buffer
    virtual int read() = 0;

    // send data to current client
    virtual int transmit(const char* msg, int len) = 0;

    // read internal buffer
    const char* get_buf() const;

protected:
    
    sockaddr_in addr; // ip address of listening socket or address to connect
    int sock = 0; // socket fd
    int port;
    char buf[MAX_MSG + 1] = {0}; // internal buffer for messages
    bool is_listening; // mode of work
};


// TCPSocket
class TCPSocket : public INETSocket {

public:
    
    // default ip - localhost
    TCPSocket(int port, bool is_listening, const char* ip="127.0.0.1");
    
    virtual int read() override;
    virtual int transmit(const char* msg, int len) override;

private:
    bool accept();

    int clients[MAX_TCP_CLIENTS] = {0}; // array of clients fd
    int curr_client = 0; // index of current client
};

// UDPSocket
class UDPSocket : public INETSocket {

public:

    UDPSocket(int port, bool is_listening, const char* ip="127.0.0.1");

    virtual int read() override;
    virtual int transmit(const char* msg, int len) override;

private:
   
    sockaddr_in client_addr;   // save address of client
    socklen_t client_addr_len; // that sent a message to answer
};