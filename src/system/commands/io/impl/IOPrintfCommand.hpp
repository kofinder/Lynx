/**
 * @file IOPrintfCommand.hpp
 * @brief Defines the IOPrintfCommand class for formatted output using printf-style syntax.
 * 
 * The IOPrintfCommand class extends IOWriteBizCommand to provide formatted printing 
 * capabilities similar to C-style `printf`. It interprets `{}` placeholders in the 
 * format string and replaces them with evaluated arguments. The first argument must 
 * always be a format string (LLVM StringType).
 * 
 * Example usage:
 * ```cpp
 * system.io.printf("Name: {}, Age: {}", name, age);
 * ```
 * 
 * This class ensures type-safe, LLVM-integrated output generation for multiple 
 * argument types, while automatically appending a newline character at the end.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_SYSTEM_IO_PRINTF_COMMAND_HPP
#define LYNX_SYSTEM_IO_PRINTF_COMMAND_HPP

#include "system/commands/io/IOWriteBizCommand.hpp"

namespace LynxSystem {

    class IOPrintfCommand final : public IOWriteBizCommand {

        public:

            IOPrintfCommand() noexcept = default;
            ~IOPrintfCommand() noexcept override = default;
        
            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();

                if (calleeArgs.empty()) return nullptr;

                auto* firstArg = calleeArgs.front();
                if (!firstArg || !TypeChecker::is<StringType>(firstArg->getType())) {
                    throw std::runtime_error("printf requires format string as first argument");
                }

                // Extract the literal string
                std::string formatLiteral = "{invalid}";
                auto formatGV = llvm::cast<llvm::GlobalVariable>(firstArg->stripPointerCasts());
                auto initializer = formatGV->getInitializer();
                if (auto* cda = llvm::dyn_cast<llvm::ConstantDataArray>(initializer)) {
                    formatLiteral = cda->getAsCString().str();
                }

                // Build final printf format string WITHOUT automatic newline
                std::string finalFmt;
                size_t argIndex = 1;

                for (size_t i = 0; i < formatLiteral.size(); ++i) {
                    if (formatLiteral[i] == '{' && i + 1 < formatLiteral.size() && formatLiteral[i + 1] == '}') {
                        if (argIndex < calleeArgs.size()) {
                            auto* arg = calleeArgs[argIndex++];
                            finalFmt += std::string(getFormatSpecifier(arg->getType()));
                            // Remove newline from specifier if exists
                            if (!finalFmt.empty() && finalFmt.back() == '\n') {
                                finalFmt.pop_back();
                            }
                        } else {
                            finalFmt += "";
                        }
                        i++; // skip closing '}'
                    } else {
                        finalFmt += formatLiteral[i];
                    }
                }


                // Prepare LLVM printf arguments
                std::vector<llvm::Value*> printfArgs;
                auto* fmtStr = builder.CreateGlobalStringPtr(finalFmt, "fmt");
                printfArgs.push_back(fmtStr);

                // Append actual argument values
                for (size_t i = 1; i < calleeArgs.size(); ++i) {
                    auto* arg = calleeArgs[i];
                    if (!arg) continue;

                    auto argsVec = preparePrintfArguments(builder, module, arg);
                    printfArgs.insert(printfArgs.end(), argsVec.begin() + 1, argsVec.end());
                }

                auto* printfFunc = getOrCreatePrintf(builder.getContext(), module);
                return builder.CreateCall(printfFunc, printfArgs, "printf_call");
            }
    };
        
}

#endif

