#include "lsp_server.hpp"
#include <iostream>

int main() {

    std::cerr << "[LynxLSPServer] Starting server..." << std::endl;

    LynxLSPServer server;

    std::cerr << "[LynxLSPServer] Entering run loop..." << std::endl;
    
    server.run();

    std::cerr << "[LynxLSPServer] Server exiting." << std::endl;

    return 0;
    
}
