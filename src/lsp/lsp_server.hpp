#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "lsp_protocol.hpp"
#include <lang/LynxDriver.hpp>

using namespace LynxLang;

/**
 * @brief Language Server Protocol (LSP) server implementation for LynxLang.
 *
 * This class implements the main loop, request dispatching,
 * and response handling for the Lynx language server.
 * It follows the JSON-RPC based Language Server Protocol (LSP).
 */
class LynxLSPServer {

    private:

        /// Underlying language driver responsible for parsing and analysis.
        std::unique_ptr<LynxDriver> driver; 

        /**
         * @brief Reads a single message from stdin (JSON-RPC over stdio).
         *
         * @return The raw JSON message as a string. Empty string if invalid.
         */
        std::string readMessage();

        /**
         * @brief Dispatches a request to the appropriate handler function.
         *
         * @param request The parsed LSP request.
         */
        void dispatch(const RequestMessage& request);

        /// Type alias for handler functions that process LSP requests.
        using HandlerFn = std::function<void(const RequestMessage&)>;

        /// Mapping of method names (e.g., "initialize") to handler functions.
        std::unordered_map<std::string, HandlerFn> handlers;

    public:

        /**
         * @brief Constructs a new LynxLSPServer and registers handlers.
        */
        LynxLSPServer();

        /**
         * @brief Main server loop. Reads and processes incoming messages.
        */
        void run();

        /**
         * @brief Sends a JSON-RPC message to the client over stdout.
         *
         * @param message The JSON object to send.
        */
        void sendMessage(const json& message);

        /**
         * @brief Handles the `initialize` request.
         *
         * @param request The incoming LSP request.
        */
        void onInitialize(const RequestMessage& request);

        /**
         * @brief Handles the `textDocument/didOpen` notification.
         *
         * @param request The incoming LSP request.
        */
        void onDidOpen(const RequestMessage& request);

        /**
         * @brief Handles the `textDocument/didChange` notification.
         *
         * @param request The incoming LSP request.
        */
        void onDidChange(const RequestMessage& request);

        /**
         * @brief Handles the `textDocument/hover` request.
         *
         * @param request The incoming LSP request.
        */
        void onHover(const RequestMessage& request);

        /**
         * @brief Handles the `textDocument/completion` request.
         *
         * @param request The incoming LSP request.
        */
        void onCompletion(const RequestMessage& request);

        /**
         * @brief Publishes diagnostics to the client.
         *
         * @param request The request/notification that triggered diagnostics.
        */
        void publishDiagnostics(const RequestMessage& request);
};
