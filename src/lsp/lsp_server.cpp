#include <iostream>
#include <string>
#include <vector>
#include "lsp_server.hpp"

#define LSP_LOG(x) std::cerr << "[LSP] " << x << std::endl;

LynxLSPServer::LynxLSPServer() {
    driver = std::make_unique<LynxDriver>();
    handlers["initialize"] = [this](const RequestMessage& r){ onInitialize(r); };
    handlers["textDocument/didOpen"] = [this](const RequestMessage& r){ onDidOpen(r); };
    handlers["textDocument/didChange"] = [this](const RequestMessage& r){ onDidChange(r); };
    handlers["textDocument/hover"] = [this](const RequestMessage& r){ onHover(r); };
    handlers["textDocument/completion"] = [this](const RequestMessage& r){ onCompletion(r); };
}

void LynxLSPServer::run() {
    while (true) {
        std::string msg = readMessage();
        if (msg.empty()) continue;

        try {
            auto jsonMsg = json::parse(msg);
            auto request = RequestMessage::fromJson(jsonMsg);
            dispatch(request);
        } catch (const std::exception& e) {
            std::cerr << "[LSP] JSON parse error: " << e.what() << std::endl;
        }
    }
}

std::string LynxLSPServer::readMessage() {
    std::string line;
    int contentLength = 0;

    while (std::getline(std::cin, line) && line != "\r") {
        if (line.rfind("Content-Length:", 0) == 0) {
            contentLength = std::stoi(line.substr(15));
        }
    }

    if (contentLength == 0) return "";

    std::vector<char> buffer(contentLength);
    std::cin.read(buffer.data(), contentLength);
    return std::string(buffer.begin(), buffer.end());
}

void LynxLSPServer::sendMessage(const json& message) {
    std::string msgStr = message.dump();
    std::cout << "Content-Length: " << msgStr.size() << "\r\n\r\n" << msgStr;
    std::cout.flush();
}

void LynxLSPServer::dispatch(const RequestMessage& request) {
    std::cerr << "[LSP] Dispatching: " << request.method << std::endl;
    if (auto it = handlers.find(request.method); it != handlers.end()) {
        it->second(request);
    } else {
        std::cerr << "[LSP] Unknown method: " << request.method << std::endl;
    }
}


void LynxLSPServer::onInitialize(const RequestMessage& request) {
    InitializeResult init;
    init.capabilities.textDocumentSync = 1;
    init.capabilities.hoverProvider = true;
    init.capabilities.completionProvider = true;
    init.capabilities.definitionProvider = true;
    init.capabilities.referencesProvider = true;

    auto response = ResponseMessage::success(request.id, init.toJson());
    sendMessage(response.toJson());
}

void LynxLSPServer::onDidOpen(const RequestMessage& request) {
    publishDiagnostics(request);
}

void LynxLSPServer::onDidChange(const RequestMessage& request) {
    publishDiagnostics(request);
}

void LynxLSPServer::onHover(const RequestMessage& request) {
    auto response = ResponseMessage::success(request.id, { {"contents", "Hover not yet implemented"} });
    sendMessage(response.toJson());
}

void LynxLSPServer::onCompletion(const RequestMessage& request) {
    auto response = ResponseMessage::success(request.id, {
        {"items", json::array()},
        {"isIncomplete", false}
    });
    sendMessage(response.toJson());
}

void LynxLSPServer::publishDiagnostics(const RequestMessage& request) {
    PublishDiagnosticsParams params;
    params.uri = request.params["textDocument"].value("uri", "");

    std::string source;

    if (request.method == "textDocument/didOpen") {
        source = request.params["textDocument"].value("text", "");
    } else if (request.method == "textDocument/didChange") {
        if (request.params.contains("contentChanges") &&
            !request.params["contentChanges"].empty()) {
            source = request.params["contentChanges"][0].value("text", "");
        }
    }

    LSP_LOG("source=" << source);
    LSP_LOG("source length=" << source.size());

    std::ifstream input(source);
    driver->setInputStream(input);
    driver->parse();

    int errCode = driver->getError();

    LSP_LOG("errCode=" << errCode);


    Diagnostic d;
    d.range = {{0, 0}, {0, 5}};
    d.severity = Severity::Success;
    d.message = "Parse source file is succesful!";
    d.source = "LynxLang";
    params.diagnostics.push_back(d);

    PublishDiagnosticsNotification notif{
        "2.0", "textDocument/publishDiagnostics", params
    };
    sendMessage(notif.toJson());
}
