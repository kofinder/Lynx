/**
 * @file IOPrintOutCommand.hpp
 * @brief Implements the IOPrintOutCommand for inline output without automatic newline.
 * 
 * The IOPrintOutCommand class extends the IOWriteBizCommand to provide functionality 
 * similar to `printf` but without automatically appending a newline. It is intended 
 * for use with prompts or inline messages where user input should appear on the 
 * same line.
 * 
 * Usage example:
 * ```
 * system.io.out("Enter your name: ");
 * string name = system.io.in();
 * ```
 * 
 * This ensures that the prompt remains on the same line as the user input.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 7, 2025
*/

#ifndef LYNX_SYSTEM_IO_PRINTOUT_COMMAND_HPP
#define LYNX_SYSTEM_IO_PRINTOUT_COMMAND_HPP

#include "system/commands/io/IOWriteBizCommand.hpp"

namespace LynxSystem {

    class IOPrintOutCommand final : public IOWriteBizCommand {

        public:

            IOPrintOutCommand() noexcept = default;
            ~IOPrintOutCommand() noexcept override = default;

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) override {
                auto& builder = context->getBuilder();
                auto* module = context->getModule();

                if (calleeArgs.empty()) {
                    auto* fmtStr = builder.CreateGlobalString("", "empty_inline");
                    return emitPrintfCall(builder, module, { fmtStr });
                }

                std::string combinedFmt;
                for (size_t i = 0; i < calleeArgs.size(); ++i) {
                    auto* arg = calleeArgs[i];
                    if (!arg) continue;
                    auto* type = arg->getType();
                    combinedFmt += std::string(getFormatSpecifier(type));
                    if (combinedFmt.ends_with('\n')) combinedFmt.pop_back(); // remove any newline
                    if (i < calleeArgs.size() - 1) combinedFmt += " "; // space between args
                }

                auto printfArgs = buildPrintfArgs(builder, module, combinedFmt, calleeArgs);
                return emitPrintfCall(builder, module, printfArgs);
            }
    };
        
}

#endif


