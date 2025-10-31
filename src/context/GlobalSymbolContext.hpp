#ifndef LYNX_GLOBAL_SYMBOL_CONTEXT_HPP
#define LYNX_GLOBAL_SYMBOL_CONTEXT_HPP

#include <memory>
#include <variant>
#include <ast/Node.hpp>
#include "VirtualTable.hpp"

#include <ast/FunctionNode.hpp>
#include <ast/FunctionCallNode.hpp>
#include <ast/StatementListNode.hpp>
#include <ast/VariableDeclarationNode.hpp>

using namespace LynxLogger;
using namespace LynxAst;

namespace LynxContext {    

    struct MethodCall { std::string funcName; std::vector<llvm::Type*> argumentTypes; };

    /**
     * @brief Centralized context for managing global symbols, vtables, and object type information.
     *
     * This class provides mechanisms to register, lookup, and manage AST nodes,
     * virtual tables, and object types across multiple modules. It also tracks
     * the current function and statement scopes and variable stacks during compilation.
     */
    class GlobalSymbolContext {

        private:

            /// Stack tracking active function nodes during code generation.
            std::stack<FunctionNode*> functions; 

            /// Stack tracking active function call nodes during code generation.
            std::stack<MethodCall*> methodCall;

            /// Vector holding currently active statement nodes.
            std::vector<Node*> statements;             

            /// Registered source folders used for module and import resolution.
            std::vector<std::string> sourceFolders;
        
            /// Mapping from class or interface names to their virtual tables.
            std::unordered_map<std::string, std::shared_ptr<VirtualTable>> vtables;

            /// AST nodes indexed by module and symbol names.
            std::unordered_map<std::string, std::unordered_map<std::string, Node*>> nodeMap;

            /// Maps module names to resolved import file paths.
            std::unordered_map<std::string, std::string> importMap;

            /// Stores variables defined in the current function or scope, 
            /// mapping variable names to their corresponding LLVM IR values.
            std::unordered_map<std::string, llvm::Value*> localVariableMap;

        public:
        
        
            /**
             * @brief Register or update an AST node under a specific module.
             * @param module Module name.
             * @param nodeName Symbol name.
             * @param node Pointer to the AST node.
             */
            void registerSymbolNode(const std::string& module, const std::string& nodeName, Node* node) { nodeMap[module][nodeName] = node; }

            /**
             * @brief Find an AST node by module and symbol name.
             * @param module Module name.
             * @param nodeName Symbol name.
             * @return Pointer to the node if found; nullptr otherwise.
             */
            Node* findSymbolNode(const std::string& module, const std::string& nodeName) const;

            /**
             * @brief Register or update a virtual table for a class or interface.
             * @param name Class or interface name.
             * @param vtable Shared pointer to the virtual table.
             */
            void registerVTable(const std::string& name, std::shared_ptr<VirtualTable> vtable) { vtables[name] = std::move(vtable); }

            /**
             * @brief Retrieve a virtual table by class or interface name.
             * @param name Class or interface name.
             * @return Shared pointer to the virtual table if found; nullptr otherwise.
             */
            std::shared_ptr<VirtualTable> findVTable(const std::string& name) const;


            /**
             * @brief Retrieve all registered AST symbol nodes.
             * @return Map of modules to their symbol maps.
             */
            inline const std::unordered_map<std::string, std::unordered_map<std::string, Node*>> getAllSymbolNodes() const { return nodeMap; }

            /**
             * @brief Register source folders used for module lookup and import resolution.
             * @param sources Vector of folder paths.
             */
            void registerSources(std::vector<std::string> sources) { sourceFolders = std::move(sources); }

            /**
             * @brief Get registered source folders.
             * @return Const reference to vector of source folder paths.
             */
            inline const std::vector<std::string>& getSources() const { return sourceFolders; }

            /**
             * @brief Associate a module name with its resolved import file path.
             * @param moduleName Module name.
             * @param filePath Resolved import file path.
             */
            void setImportFile(const std::string& moduleName, const std::string& filePath) { importMap[moduleName] = filePath; }

            /**
             * @brief Get all module-to-import file mappings.
             * @return Const reference to the import map.
             */
            inline const std::unordered_map<std::string, std::string>& getImportFiles() const { return importMap; }

            /**
             * @brief Check if a module is already imported.
             * @param moduleName Module name to check.
             * @return True if imported; false otherwise.
             */
            inline bool hasImportedModule(const std::string& moduleName) const { return importMap.find(moduleName) != importMap.end(); }

            /**
             * @brief Find module name by import file path.
             * @param filePath Import file path.
             * @return Optional containing module name if found, nullopt otherwise.
             */
            std::optional<std::string> findImportKeyByValue(const std::string& filePath) const;

            /**
             * @brief Find import file path by module name.
             * 
             * Searches the import map for a given module name and retrieves its associated
             * resolved file path if present.
             * 
             * @param moduleName The name of the module to search for.
             * @return std::optional<std::string> containing the file path if found, 
             *         or std::nullopt if the module is not in the import map.
             */
            std::optional<std::string> findImportValueByKey(const std::string& moduleName) const;

            /**
             * @brief Push a function node onto the function stack.
             * 
             * Adds a function node to the stack of currently active functions.
             * 
             * @param fnNode Pointer to the FunctionNode to push.
            */
            inline void addFunction(FunctionNode* fnNode) { functions.push(fnNode); }

            /**
             * @brief Push a Method Call Node onto the function stack.
             * 
             * Adds a Method Call Node to the stack of currently active functions.
             * 
             * @param fnNode Pointer to the FunctionNode to push.
            */
            inline void addMethodCall(MethodCall* method) { methodCall.push(method); }

            /**
             * @brief Pop the top function node from the stack.
             * 
             * Removes the most recently added function node from the active function stack.
             * This is a no-op if the stack is empty.
             */
            inline void removeFunction() { if (!functions.empty()) functions.pop(); }

            /**
             * @brief Pop the top function call node from the stack.
             * 
             * Removes the most recently added function call node from the active function stack.
             * This is a no-op if the stack is empty.
             */
            inline void removeMethodCall() { if (!methodCall.empty()) methodCall.pop(); }

            /**
             * @brief Add a statement node to the current statement list.
             * 
             * Appends a new statement node to the list of statements being tracked.
             * 
             * @param stmtNode Pointer to the statement node to add.
             */
            inline void addStatement(Node* stmtNode) { statements.push_back(stmtNode); }

            /**
             * @brief Remove the most recent statement node.
             * 
             * Removes the last statement node from the list of tracked statements.
             * This is a no-op if the list is empty.
             */
            inline void removeStatement() { if (!statements.empty()) statements.pop_back(); }

            /**
             * @brief Get the current (top) function node.
             * @return Pointer to the function node or nullptr if stack empty.
             */
            FunctionNode* getFunctionNode() const { return functions.empty() ? nullptr : functions.top(); }

             /**
             * @brief Get the current (top) method call node.
             * @return Pointer to the function call node or nullptr if stack empty.
             */
            MethodCall* getMethodCall() const { return methodCall.empty() ? nullptr : methodCall.top(); }


           /**
             * @brief Get the most recent statement node.
             * 
             * Returns the last statement node added to the AST context.
             * 
             * @return Pointer to the most recent statement node, or nullptr if none exist.
             */
            Node* getStatementNode() const { return statements.empty() ? nullptr : statements.back(); }

            /**
             * @brief Search for a local variable declaration by name.
             * 
             * Looks for a variable declaration within the current set of statements.
             * 
             * @param name Variable name to search for.
             * @return Pointer to VariableDeclarationNode if found, nullptr otherwise.
             */
            VariableDeclarationNode* findVariable(const std::string& name) const;

            /**
             * @brief Add a local variable.
             * 
             * Stores a new variable in the local variables map.
             * 
             * @param varName Name of the variable.
             * @param value LLVM IR value associated with the variable.
             */
            void registerLocalVariable(const std::string& varName, llvm::Value* value) { localVariableMap[varName] = value;  }

            /**
             * @brief Find a local variable.
             * 
             * Searches for a local variable by name within the current scope.
             * 
             * @param varName Name of the variable.
             * @return LLVM IR value if found, nullptr otherwise.
             */
            llvm::Value* findLocalVariable(const std::string& varName) const;

            /**
             * @brief Clear all local variables.
             * 
             * Removes all stored local variables from the current scope.
             */
            void clearLocalVariables() { localVariableMap.clear(); }

    };
}

#endif
