#ifndef LYNX_SYSTEM_IO_WRITE_COMMAND_BASE_HPP
#define LYNX_SYSTEM_IO_WRITE_COMMAND_BASE_HPP

#include <unordered_map>
#include <functional>
#include "system/ISystemCommand.hpp"
#include "utils/SystemModuleUtils.hpp"

namespace LynxSystem {

    using namespace LynxSystem::utils;

    class IOWriteBizCommand : public ISystemCommand {

        public:

            IOWriteBizCommand() noexcept = default;
            ~IOWriteBizCommand() noexcept override = default;

        protected:

            /**
             * @brief Creates an LLVM call to `printf()` for the given arguments.
             * 
             * This utility abstracts the common call creation shared across
             * println, printf, and future write operations.
             *
             * @param builder The current LLVM IRBuilder.
             * @param module  The active LLVM module.
             * @param printfArgs A vector of LLVM arguments, starting with format string.
             * @return llvm::Value* The resulting call instruction.
            */
            llvm::Value* emitPrintfCall(llvm::IRBuilder<>& builder, llvm::Module* module, const std::vector<llvm::Value*>& printfArgs) const noexcept {
                if (printfArgs.empty()) return nullptr;
                auto* printfFunc = getOrCreatePrintf(builder.getContext(), module);
                return builder.CreateCall(printfFunc, printfArgs, "io_printf_call");    
            }

            /**
             * @brief Concatenates format specifiers for a given list of arguments.
             * 
             * Automatically inserts spaces between format specifiers and ensures
             * the string ends with a newline when requested.
            */
            std::string buildFormatString(const std::vector<llvm::Value*>& args, bool appendNewline = true) const noexcept {
                std::string format;
                for (size_t i = 0; i < args.size(); ++i) {
                    auto* arg = args[i];
                    if (!arg) continue;
                    format += std::string(getFormatSpecifier(arg->getType()));
                    if (format.ends_with('\n')) format.pop_back();
                    if (i < args.size() - 1) format += " ";
                }
                if (appendNewline) format += "\n";
                return format;
            }

            /**
             * @brief Builds a full printf argument list from given expressions.
             * 
             * The first element is always the global format string constant.
            */
            std::vector<llvm::Value*> buildPrintfArgs(llvm::IRBuilder<>& builder, llvm::Module* module, const std::string& fmt, const std::vector<llvm::Value*>& args) const noexcept {
                std::vector<llvm::Value*> printfArgs;
                printfArgs.push_back(builder.CreateGlobalStringPtr(fmt, "fmt"));
                for (auto* arg : args) {
                    if (!arg) continue;
                    auto prepared = preparePrintfArguments(builder, module, arg);
                    for (size_t i = 1; i < prepared.size(); ++i) {
                        printfArgs.push_back(prepared[i]);
                    }
                }

                return printfArgs;
            }
    
    };
        
}

#endif
