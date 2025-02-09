#include "HTTPServer.h"

HTTPServer::HTTPServer(const int port, const size_t num_threads) : tcp_server(port), thread_pool(num_threads) {}

std::string HTTPServer::HandleRequest(const std::string &request) {
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/html\r\n"
           "Content-Length: 13\r\n"
           "\r\n"
           "Hello, World!";
}

void HTTPServer::ProcessClient(const int client_socket) {
    char buffer[1024] = {0};
    auto bytes_received = tcp_server.Receive(client_socket, buffer, sizeof(buffer));

    if (bytes_received > 0) {
        const std::string request(buffer);
        const std::string response = HandleRequest(request);
        tcp_server.SendResponse(client_socket, response);
    }

    tcp_server.CloseSocket(client_socket);
}


void HTTPServer::run() {
    if (!tcp_server.Start()) return;

    while (true) {
        int client_socket = tcp_server.AcceptClient();
        if (client_socket < 0) continue;

        thread_pool.EnqueueTask([this, client_socket]() {
            ProcessClient(client_socket);
        });
    }
}

