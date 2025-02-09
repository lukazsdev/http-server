#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "TCPServer.h"
#include <string>

#include "ThreadPool.h"

class HTTPServer {
public:
    explicit HTTPServer(int port, size_t num_threads);
    void run(); // Start server

private:
    TCPServer tcp_server; // Manage tcp connections
    ThreadPool thread_pool; // Handle clients

    static std::string HandleRequest(const std::string& request) ; // Process an HTTP request
    static void ProcessClient(int client_socket) ; // Handle a single client connection

    [[nodiscard]] static std::string GenerateStatsResponse();
    [[nodiscard]] static std::string GenerateMainResponse();
    [[nodiscard]] static std::string GenerateErrorResponse();
};



#endif //HTTPSERVER_H
