#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

class TCPServer {
public:
    explicit TCPServer(int port);
    ~TCPServer();

    bool Start();
    int AcceptClient() const;
    ssize_t Receive(int client_socket, char* buffer, size_t buffer_size);
    ssize_t SendResponse(int client_socket, const std::string& response);
    void CloseSocket(int socket);

private:
    int server_fd; // Server socket file descriptor
    int port;
};



#endif //TCPSERVER_H
