/**
 * @file LLVMIOUtils.hpp
 * @brief Utility functions and type-safe helpers for I/O operations in LLVM IR.
 *
 * This header defines a collection of low-level LLVM utilities used by
 * Lynx’s system modules to perform input and output operations (e.g., printf/scanf).
 * It includes type-aware format specifier generation, argument preparation,
 * and code emission utilities for printing or reading values in LLVM IR.
 *
 * The utilities rely on Lynx’s `TypeChecker` and `TypeCaster` templates
 * to associate high-level language types with LLVM IR types, ensuring that
 * generated I/O calls are both type-correct and semantically consistent.
 *
 * Key features:
 * - Compile-time type introspection via `LLVMTypeCheckable` concept.
 * - Automatic format specifier inference for Lynx language types.
 * - LLVM IR builder utilities for `printf` and `scanf` creation.
 * - Type-specific printing and reading of primitive and composite types.
 *
 * @note These functions are part of the internal LLVM code generation layer
 *       and are not intended for direct use outside of system module contexts.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
 */

#ifndef LYNX_SYSTEM_MODULE_UTILS_HPP
#define LYNX_SYSTEM_MODULE_UTILS_HPP

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>
#include <types/tmpl/TypeChecker.hpp>
#include <types/tmpl/TypeCaster.hpp>
#include <llvm/Support/raw_ostream.h>

namespace LynxSystem::utils {

    using namespace LynxTypes;

    /**
     * @brief Concept to ensure that a type can be validated through TypeChecker.
     *
     * Requires that `TypeChecker::is<T>(llvm::Type*)` exists and returns a boolean.
    */
    template <typename T>
    concept LLVMTypeCheckable = requires(llvm::Type* type) {
        { TypeChecker::is<T>(type) } -> std::convertible_to<bool>;
    };

    /**
     * @brief Returns a printf-style format specifier for a given LLVM type.
     *
     * Determines the appropriate format string for printing a value of
     * the provided LLVM type. It performs runtime type checks using
     * `TypeChecker::is<T>()` for all supported Lynx types.
     *
     * @param valueType LLVM type of the value being printed.
     * @return A string_view containing the format specifier.
    */
    [[nodiscard]] inline constexpr std::string_view getFormatSpecifier(llvm::Type* const valueType) noexcept {
        if (!valueType) return "%p\n";
        if (TypeChecker::is<StringType>(valueType))      return "%s\n";
        if (TypeChecker::is<BooleanType>(valueType))     return "%s\n";
        if (TypeChecker::is<CharType>(valueType))        return "%c\n";
        if (TypeChecker::is<ByteType>(valueType))        return "%d\n";
        if (TypeChecker::is<ShortType>(valueType))       return "%d\n";
        if (TypeChecker::is<IntegerType>(valueType))     return "%d\n";
        if (TypeChecker::is<LongType>(valueType))        return "%ld\n";
        if (TypeChecker::is<FloatType>(valueType))       return "%f\n";
        if (TypeChecker::is<DoubleType>(valueType))      return "%lf\n";
        if (TypeChecker::is<EnumType>(valueType))        return "Enum: [index: %d, value: %c, name: %s]\n";
        if (TypeChecker::is<DateType>(valueType))        return "%04d-%02d-%02d\n";
        if (TypeChecker::is<DateTimeType>(valueType))    return "%04d-%02d-%02dT%02d:%02d:%02d.%03d\n";
        if (TypeChecker::is<ClassType>(valueType))       return "%p\n";
        if (TypeChecker::is<ArrayType>(valueType))       return "[array]\n";
        if (TypeChecker::is<FileType>(valueType))        return "[file]\n";

        std::cerr << "[TypeChecker] Warning: Unrecognized type for format specifier\n";
        return "%p\n";
    }

    /**
     * @brief Compile-time retrieval of format specifier for a given Lynx type.
     *
     * @tparam T  A type that satisfies the LLVMTypeCheckable concept.
     * @return The corresponding printf format string.
    */
    template <LLVMTypeCheckable T>
    [[nodiscard]] constexpr std::string_view getFormatSpecifierFor() noexcept {
        if constexpr (std::is_same_v<T, StringType>)      return "%s\n";
        else if constexpr (std::is_same_v<T, BooleanType>) return "%s\n";
        else if constexpr (std::is_same_v<T, CharType>)    return "%c\n";
        else if constexpr (std::is_same_v<T, ByteType> ||
                        std::is_same_v<T, ShortType> ||
                        std::is_same_v<T, IntegerType>) return "%d\n";
        else if constexpr (std::is_same_v<T, LongType>)    return "%ld\n";
        else if constexpr (std::is_same_v<T, FloatType>)   return "%f\n";
        else if constexpr (std::is_same_v<T, DoubleType>)  return "%lf\n";
        else if constexpr (std::is_same_v<T, EnumType>)    return "Enum: [index: %d, value: %c, name: %s]\n";
        else if constexpr (std::is_same_v<T, DateType>)    return "%04d-%02d-%02d\n";
        else if constexpr (std::is_same_v<T, DateTimeType>)return "%04d-%02d-%02dT%02d:%02d:%02d.%03d\n";
        else if constexpr (std::is_same_v<T, ClassType>)   return "%p\n";
        else if constexpr (std::is_same_v<T, ArrayType>)   return "[array]\n";
        else if constexpr (std::is_same_v<T, FileType>)    return "[file]\n";
        else return "%p\n";
    }

    /**
     * @brief Prepares argument list for a `printf` call based on the given value.
     *
     * Automatically constructs the vector of arguments required for calling
     * `printf()` in LLVM IR. Handles type-specific transformations (e.g.,
     * boolean to string, float promotion, struct field extraction).
     *
     * @param builder IRBuilder used to generate the call.
     * @param module  LLVM module where the global string constants will be created.
     * @param expressionValue LLVM value representing the expression to print.
     * @return Vector of LLVM values representing `printf` arguments.
    */
    [[nodiscard]] inline std::vector<llvm::Value*> preparePrintfArguments(
        llvm::IRBuilder<>& builder, 
        llvm::Module* module, 
        llvm::Value* expressionValue
    ) noexcept(false) {

        auto* llvmType = expressionValue->getType();
        std::vector<llvm::Value*> printfArgs;

        llvmType->print(llvm::outs());

        const auto formatSpecifier = getFormatSpecifier(llvmType);
        auto* formatString = builder.CreateGlobalStringPtr(std::string(formatSpecifier), "fmt");
        printfArgs.push_back(formatString);

        if(TypeChecker::is<BooleanType>(llvmType)) {
            auto* booleanAsString = builder.CreateSelect(
                expressionValue, 
                builder.CreateGlobalStringPtr("true"),
                builder.CreateGlobalStringPtr("false")
            );
            printfArgs.push_back(booleanAsString);
        } else if(TypeChecker::is<FloatType>(llvmType)) {
            auto* floatPromotion = builder.CreateFPExt(expressionValue, builder.getDoubleTy(), "promotedFloat");
            printfArgs.push_back(floatPromotion);
        } else if(TypeChecker::is<DateTimeType>(llvmType)) {

            llvm::StructType* structTy = nullptr;
            if (auto* ptrType = llvm::dyn_cast<llvm::PointerType>(llvmType)) {
                structTy = llvm::dyn_cast<llvm::StructType>(ptrType->getPointerElementType());
            } else {
                structTy = llvm::dyn_cast<llvm::StructType>(llvmType);
            }

            if (!structTy) throw std::runtime_error("Expected DateTime struct type");

            // Extract fields in order
            llvm::Value* yearPtr = builder.CreateStructGEP(structTy, expressionValue, 0);
            llvm::Value* year = builder.CreateLoad(builder.getInt32Ty(), yearPtr);

            llvm::Value* monthPtr = builder.CreateStructGEP(structTy, expressionValue, 1);
            llvm::Value* month = builder.CreateLoad(builder.getInt32Ty(), monthPtr);

            llvm::Value* dayPtr = builder.CreateStructGEP(structTy, expressionValue, 2);
            llvm::Value* day = builder.CreateLoad(builder.getInt32Ty(), dayPtr);

            llvm::Value* hourPtr = builder.CreateStructGEP(structTy, expressionValue, 3);
            llvm::Value* hour = builder.CreateLoad(builder.getInt32Ty(), hourPtr);

            llvm::Value* minutePtr = builder.CreateStructGEP(structTy, expressionValue, 4);
            llvm::Value* minute = builder.CreateLoad(builder.getInt32Ty(), minutePtr);

            llvm::Value* secondPtr = builder.CreateStructGEP(structTy, expressionValue, 5);
            llvm::Value* second = builder.CreateLoad(builder.getInt32Ty(), secondPtr);

            llvm::Value* milliPtr = builder.CreateStructGEP(structTy, expressionValue, 6);
            llvm::Value* millisecond = builder.CreateLoad(builder.getInt32Ty(), milliPtr);

            printfArgs.push_back(year);
            printfArgs.push_back(month);
            printfArgs.push_back(day);
            printfArgs.push_back(hour);
            printfArgs.push_back(minute);
            printfArgs.push_back(second);
            printfArgs.push_back(millisecond);

        } else if(TypeChecker::is<EnumType>(llvmType)) {
            auto enumValue = builder.CreateExtractValue(expressionValue, {1}, "enum_value");
            auto intValue = builder.CreateExtractValue(enumValue, {0}, "id");
            auto charValue = builder.CreateExtractValue(enumValue, {1}, "name");
            auto stringValue = builder.CreateExtractValue(enumValue, {2}, "value");
            printfArgs.push_back(intValue);
            printfArgs.push_back(charValue);
            printfArgs.push_back(stringValue);

        } else if(TypeChecker::is<CharType>(llvmType)) {
            llvm::Value* charVal = nullptr;
            if (auto* ptrType = llvm::dyn_cast<llvm::PointerType>(llvmType)) {
                auto* structTy = llvm::dyn_cast<llvm::StructType>(ptrType->getPointerElementType());
                if (!structTy) throw std::runtime_error("Expected Char struct type pointer");
                auto* fieldPtr = builder.CreateStructGEP(structTy, expressionValue, 0, "char_field_ptr");
                charVal = builder.CreateLoad(builder.getInt8Ty(), fieldPtr, "char_load_val");
            } else if (auto* structTy = llvm::dyn_cast<llvm::StructType>(llvmType)) {
                charVal = builder.CreateExtractValue(expressionValue, {0}, "char_extract_val");
            } else if (llvmType->isIntegerTy(8)) {
                charVal = expressionValue;
            } else {
                throw std::runtime_error("Unsupported CharType representation");
            }
        
            // Promote to i32 for printf vararg
            auto* charValInt = builder.CreateSExt(charVal, builder.getInt32Ty(), "char_promoted");
            printfArgs.push_back(charValInt);
        } else {
            printfArgs.push_back(expressionValue);
        }

        return printfArgs;
    }


    /**
     * @brief Retrieves (or inserts) the `printf` function declaration into the module.
    */
    [[nodiscard]] inline llvm::Function* getOrCreatePrintf(llvm::LLVMContext& context, llvm::Module* module) noexcept {
        constexpr bool isVarArg = true;
        auto* printfType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context),
            { llvm::Type::getInt8PtrTy(context) },
            isVarArg
        );
        return llvm::cast<llvm::Function>(module->getOrInsertFunction("printf", printfType).getCallee());
    }
    
    /**
     * @brief Retrieves (or inserts) the `scanf` function declaration into the module.
    */
    [[nodiscard]] inline llvm::Function* getOrCreateScanf(llvm::LLVMContext& context, llvm::Module* module) noexcept {
        constexpr bool isVarArg = true;
        auto* scanfType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context),
            { llvm::Type::getInt8PtrTy(context) },
            isVarArg
        );
        return llvm::cast<llvm::Function>(module->getOrInsertFunction("scanf", scanfType).getCallee());
    }

}

#endif
