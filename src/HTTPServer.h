#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "TCPServer.h"
#include <string>

class HTTPServer {
public:
    explicit HTTPServer(int port);
    void run();

private:
    TCPServer tcp_server;
    std::string HandleRequest(const std::string& request);
};



#endif //HTTPSERVER_H
