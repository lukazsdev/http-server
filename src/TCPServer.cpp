#include "TCPServer.h"

TCPServer::TCPServer(const int port) : server_fd(-1), port(port) {}

TCPServer::~TCPServer() {
    if (server_fd != -1) {
        close(server_fd);
    }
}

bool TCPServer::Start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return false;
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return false;
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        return false;
    }

    std::cout << "Server listening on port " << port << "...\n";
    return true;
}

int TCPServer::AcceptClient() const {
    struct sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

    if (client_socket < 0) {
        perror("Accept failed");
    }

    return client_socket;
}

ssize_t TCPServer::Receive(int client_socket, char *buffer, size_t buffer_size) {
    return read(client_socket, buffer, buffer_size);
}

ssize_t TCPServer::SendResponse(int client_socket, const std::string &response) {
    return write(client_socket, response.c_str(), response.length());
}

void TCPServer::CloseSocket(int socket) {
    close(socket);
}




