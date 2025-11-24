/**
 * @file DateTimeUtils.hpp
 * @brief Utility functions to generate LLVM DateTime struct values during code generation.
 * 
 * The DateTimeUtils namespace provides helpers to construct LLVM constants representing
 * DateTime values, either from the current local time or from constructor arguments.
 * 
 * **Key Responsibilities:**
 * - Generate LLVM constant values and types for DateTime structs.
 * - Support current local time and argument-based initialization.
 * 
 * **Used By:**
 * - Code generation routines for DateTime literals and constructor expressions.
 * 
 * @see AstContext, ExpressionNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/

#ifndef LYNX_DATE_TIME_UTILS
#define LYNX_DATE_TIME_UTILS

#include <llvm/IR/Value.h>
#include <logger/Logger.hpp>
#include <context/AstContext.hpp>
#include "ExpressionNode.hpp"
#include <ctime>


namespace LynxAst::DateTimeUtils {
    
    using namespace LynxLogger;
    using namespace LynxContext;


    /**
     * @brief Creates constant LLVM values and types for a DateTime struct
     *        using the current local time.
     * 
     * @param astContext The current AST context.
     * @return A pair of vectors: first = LLVM values, second = LLVM types.
    */
    inline std::pair<std::vector<llvm::Value*>, std::vector<llvm::Type*>> generateCurrentDateTime(llvm::LLVMContext& context) {
        std::time_t t = std::time(nullptr);
        std::tm localTime{};
        #ifdef _WIN32
            localtime_s(&localTime, &t);
        #else
            localtime_r(&t, &localTime);
        #endif

        int year = localTime.tm_year + 1900;
        int month = localTime.tm_mon + 1;
        int day = localTime.tm_mday;
        int hour = localTime.tm_hour;
        int minute = localTime.tm_min;
        int second = localTime.tm_sec;
        int millisecond = 0;
        int timezoneOffset = 0;

    
        auto* i32Ty = llvm::Type::getInt32Ty(context);
        std::vector<int> values = { year, month, day, hour, minute, second, millisecond, timezoneOffset };

        std::vector<llvm::Value*> llvmValues;
        std::vector<llvm::Type*> llvmTypes;
    
        for (int v : values) {
            llvmValues.push_back(llvm::ConstantInt::get(i32Ty, v));
            llvmTypes.push_back(i32Ty);
        }
    
        return { llvmValues, llvmTypes };    
    }


    /**
     * @brief Creates constant LLVM values and types for a DateTime struct
     *        from constructor arguments.
     * 
     * @param astContext The AST context.
     * @param args List of constructor arguments (should be constants).
     * @return A pair of vectors: first = LLVM values, second = LLVM types.
    */
    inline std::pair<std::vector<llvm::Value*>, std::vector<llvm::Type*>> generateDateTime(const AstContext& astContext, std::vector<std::unique_ptr<ExpressionNode>>& args) {

        constexpr int kExpectedFields = 8;
        auto& ctx = astContext.getLLVMContext();
        llvm::Type* i32Ty = llvm::Type::getInt32Ty(ctx);
    
        std::vector<llvm::Value*> llvmValues;
        std::vector<llvm::Type*> llvmTypes;

        if (args.size() < 3) {
            LOG_ERROR("DateTime requires at least 3 constructor arguments: year, month, day");
            return {llvmValues, llvmTypes};
        }

        for (int i = 0; i < kExpectedFields; ++i) {
            if (i < static_cast<int>(args.size())) {
                auto val = args[i]->generateCode(astContext.createContext());
                if (auto* constInt = llvm::dyn_cast<llvm::ConstantInt>(val)) {
                    llvmValues.push_back(constInt);
                    llvmTypes.push_back(i32Ty);
                } else {
                    LOG_ERROR("DateTime argument {} is not a constant integer", i);
                    llvmValues.push_back(llvm::ConstantInt::get(i32Ty, 0));
                    llvmTypes.push_back(i32Ty);
                }
            } else {
                // Fill missing values with zero
                llvmValues.push_back(llvm::ConstantInt::get(i32Ty, 0));
                llvmTypes.push_back(i32Ty);
            }
        }
    
        return { llvmValues, llvmTypes };    
    }
    
}

#endif
