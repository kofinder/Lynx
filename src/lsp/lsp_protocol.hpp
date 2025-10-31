#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

// ----------------- Severity -----------------
enum class Severity {
    Error,
    Warning,
    Info,
    Success // maps to LSP "Hint"
};

inline int severityToLSP(Severity s) {
    switch (s) {
        case Severity::Error:   return 1;
        case Severity::Warning: return 2;
        case Severity::Info:    return 3;
        case Severity::Success: return 4;
    }
    return 3; // default Info
}

// ----------------- Position -----------------
struct Position {
    int line = 0;
    int character = 0;

    json toJson() const {
        return { {"line", line}, {"character", character} };
    }
};

// ----------------- Range -----------------
struct Range {
    Position start;
    Position end;

    json toJson() const {
        return { {"start", start.toJson()}, {"end", end.toJson()} };
    }
};

// ----------------- Diagnostic -----------------
struct Diagnostic {
    Range range;
    Severity severity = Severity::Info;
    std::string message;
    std::string source;

    json toJson() const {
        return {
            {"range", range.toJson()},
            {"severity", severityToLSP(severity)},
            {"message", message},
            {"source", source}
        };
    }
};

// ----------------- TextDocument -----------------
struct TextDocument {
    std::string uri;
    std::string text;

    TextDocument() = default;
    TextDocument(const std::string& u, const std::string& t) 
        : uri(u), text(t) {}
};

// ----------------- Capabilities -----------------
struct ServerCapabilities {
    int textDocumentSync = 1;   // 1 = Full sync
    bool hoverProvider = true;
    bool completionProvider = true;
    bool completionResolveProvider = false;
    bool definitionProvider = true;
    bool referencesProvider = true;
    bool diagnosticInterFileDependencies = false;
    bool diagnosticWorkspaceDiagnostics = false;

    json toJson() const {
        return {
            {"textDocumentSync", textDocumentSync},
            {"hoverProvider", hoverProvider},
            {"completionProvider", {
                {"resolveProvider", completionResolveProvider}
            }},
            {"definitionProvider", definitionProvider},
            {"referencesProvider", referencesProvider},
            {"diagnosticProvider", {
                {"interFileDependencies", diagnosticInterFileDependencies},
                {"workspaceDiagnostics", diagnosticWorkspaceDiagnostics}
            }}
        };
    }
};

struct InitializeResult {
    ServerCapabilities capabilities;

    json toJson() const {
        return { {"capabilities", capabilities.toJson()} };
    }
};

// ----------------- Diagnostics Notification -----------------
struct PublishDiagnosticsParams {
    std::string uri;
    std::vector<Diagnostic> diagnostics;

    json toJson() const {
        json diags = json::array();
        for (const auto& d : diagnostics) {
            diags.push_back(d.toJson());
        }
        return {
            {"uri", uri},
            {"diagnostics", diags}
        };
    }
};

struct PublishDiagnosticsNotification {
    std::string jsonrpc = "2.0";
    std::string method = "textDocument/publishDiagnostics";
    PublishDiagnosticsParams params;

    json toJson() const {
        return {
            {"jsonrpc", jsonrpc},
            {"method", method},
            {"params", params.toJson()}
        };
    }
};

// ----------------- Request / Response -----------------
struct RequestMessage {
    std::string jsonrpc = "2.0"; 
    int id = 0;                    // Optional (0 means no id = notification)
    std::string method;            // e.g. "initialize", "textDocument/didOpen"
    json params;                   // The method params

    static RequestMessage fromJson(const json& j) {
        RequestMessage msg;
        msg.jsonrpc = j.value("jsonrpc", "2.0");
        msg.id      = j.contains("id") ? j["id"].get<int>() : 0;
        msg.method  = j.value("method", "");
        if (j.contains("params")) {
            msg.params = j["params"];
        }
        return msg;
    }

    bool isNotification() const {
        return id == 0; // LSP notifications have no "id"
    }
};

struct ResponseMessage {
    std::string jsonrpc = "2.0";  // always "2.0" in LSP
    int id = 0;                   // request id
    json result;                  // successful result
    json error;                   // error object (if any)

    static ResponseMessage success(int id, const json& result) {
        ResponseMessage msg;
        msg.id = id;
        msg.result = result;
        return msg;
    }

    static ResponseMessage failure(int id, int code, const std::string& message) {
        ResponseMessage msg;
        msg.id = id;
        msg.error = {
            {"code", code},
            {"message", message}
        };
        return msg;
    }

    json toJson() const {
        json j;
        j["jsonrpc"] = jsonrpc;
        j["id"] = id;
        if (!error.is_null()) {
            j["error"] = error;
        } else {
            j["result"] = result;
        }
        return j;
    }
};
