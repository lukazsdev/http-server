#include "src/HTTPServer.h"

int main() {
    HTTPServer server(8080);
    server.run();

    return 0;
}
