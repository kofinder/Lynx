/**
 * @file IOReadBizCommand.hpp
 * @brief Base class for handling input (read) operations in Lynx system modules.
 *
 * The IOReadBizCommand provides reusable helper utilities for building
 * LLVM IR code that performs formatted input operations. It exposes a
 * protected helper to emit a `scanf` call that reads a value of a
 * specific type from standard input.
 *
 * Derived classes such as IOReadLineCommand or IOReadIntCommand extend
 * this to handle specific data types.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_READ_COMMAND_BASE_HPP
#define LYNX_SYSTEM_IO_READ_COMMAND_BASE_HPP

#include <unordered_map>
#include <functional>
#include <types/tmpl/TypeCaster.hpp>
#include "system/ISystemCommand.hpp"
#include "utils/SystemModuleUtils.hpp"

namespace LynxSystem {

    using namespace LynxSystem::utils;
    using namespace LynxTypes;

    class IOReadBizCommand : public ISystemCommand {

        public:

            IOReadBizCommand() noexcept = default;
            ~IOReadBizCommand() noexcept override = default;

        protected:

            /**
             * @brief Emits an LLVM IR call to `scanf()` to read a single value.
             *
             * This utility builds and inserts the IR instructions required to:
             * - Allocate a temporary variable on the stack
             * - Emit a `scanf` call using the specified format string
             * - Load and return the read value
             *
             * @param builder    Reference to LLVM IRBuilder.
             * @param module     Target LLVM Module.
             * @param fmt        The scanf format string.
             * @param valueType  LLVM type to read into.
             * @return LLVM Value representing the loaded input.
            */
            [[nodiscard]] llvm::Value* emitScanfRead(llvm::IRBuilder<>& builder, llvm::Module* module, const std::string& fmt, llvm::Type* valueType) const noexcept {
    
                auto* scanfFunc = getOrCreateScanf(builder.getContext(), module);

                auto* formatStr = builder.CreateGlobalString(fmt, "scanf_fmt");
            
                auto* tmpVar = builder.CreateAlloca(valueType, nullptr, "scanf_tmp");
            
                builder.CreateCall(scanfFunc, {formatStr, tmpVar}, "scanf_call");
                        
                return builder.CreateLoad(valueType, tmpVar, "read_value");
            
            }

    };
        
}

#endif
