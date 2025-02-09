#include "HTTPServer.h"

#include <sstream>

HTTPServer::HTTPServer(const int port, const size_t num_threads) : tcp_server(port), thread_pool(num_threads) {}

std::string HTTPServer::HandleRequest(const std::string &request) {
    std::istringstream request_stream(request);
    std::string method, path, protocol;
    request_stream >> method >> path >> protocol;
    std::cout << method << " " << path << " " << protocol << std::endl;

    if (path == "/") return GenerateMainResponse();
    else if (path == "/stats") return GenerateStatsResponse();
    else return GenerateErrorResponse();
}

void HTTPServer::ProcessClient(const int client_socket) {
    char buffer[1024] = {0};
    auto bytes_received = TCPServer::Receive(client_socket, buffer, sizeof(buffer));

    if (bytes_received > 0) {
        const std::string request(buffer);
        const std::string response = HandleRequest(request);
        TCPServer::SendResponse(client_socket, response);
    }

    TCPServer::CloseSocket(client_socket);
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

std::string HTTPServer::GenerateMainResponse() {
    return "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Content-Length: 13\r\n"
               "\r\n"
               "Hello, World!";
}

std::string HTTPServer::GenerateStatsResponse() {
    static int request_count = 0;
    request_count++;

    const std::string response_body = "Total requests handled: " + std::to_string(request_count);
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: " + std::to_string(response_body.size()) + "\r\n"
           "\r\n" + response_body;
}


std::string HTTPServer::GenerateErrorResponse() {
    return "HTTP/1.1 400 Bad Request\r\n";
}

