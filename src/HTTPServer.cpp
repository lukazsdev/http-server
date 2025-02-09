#include "HTTPServer.h"

HTTPServer::HTTPServer(int port) : tcp_server(port) {}

std::string HTTPServer::HandleRequest(const std::string &request) {
    // Very basic request handling (just returns Hello, World!)
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/html\r\n"
           "Content-Length: 13\r\n"
           "\r\n"
           "Hello, World!";
}

void HTTPServer::run() {
    if (!tcp_server.Start()) return;

    while (true) {
        int client_socket = tcp_server.AcceptClient();
        if (client_socket < 0) continue;

        char buffer[1024] = {0};
        ssize_t bytes_received = tcp_server.Receive(client_socket, buffer, sizeof(buffer));

        if (bytes_received > 0) {
            std::string request(buffer);
            std::string response = HandleRequest(request);
            tcp_server.SendResponse(client_socket, response);
        }

        tcp_server.CloseSocket(client_socket);
    }
}

