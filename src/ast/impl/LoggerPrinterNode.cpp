#include "LoggerPrinterNode.hpp"
#include <types/tmpl/TypeChecker.hpp>
#include <types/tmpl/TypeCaster.hpp>

#include <llvm/Support/raw_ostream.h>


using namespace LynxContext;
using namespace LynxConstants;
using namespace LynxTypes;

namespace LynxAst {


    std::string getFormatSpecifier(llvm::Type* valueType) {

        if (!valueType) {
            std::cerr << "[TypeChecker] Warning: null type passed to getFormatSpecifier\n";
            return "%p\n";
        }

        if (TypeChecker::is<StringType>(valueType))      return "%s\n";
        if (TypeChecker::is<BooleanType>(valueType))     return "%s\n";  // true/false as 0/1
        if (TypeChecker::is<CharType>(valueType))        return "%c\n";
        if (TypeChecker::is<ByteType>(valueType))        return "%d\n";
        if (TypeChecker::is<ShortType>(valueType))       return "%d\n";
        if (TypeChecker::is<IntegerType>(valueType))     return "%d\n";
        if (TypeChecker::is<LongType>(valueType))        return "%ld\n";
        if (TypeChecker::is<FloatType>(valueType))       return "%f\n";
        if (TypeChecker::is<DoubleType>(valueType))      return "%lf\n";
        if (TypeChecker::is<EnumType>(valueType))        return "Enum: [index: %d, value: %c, name: %s]\n";
        if (TypeChecker::is<DateType>(valueType))        return "%04d-%02d-%02d\n";  // year-month-day
        if (TypeChecker::is<DateTimeType>(valueType))    return "%04d-%02d-%02dT%02d:%02d:%02d.%03d\n";  // ISO-like
        if (TypeChecker::is<ClassType>(valueType))       return "%p\n";  // fallback to pointer address
        if (TypeChecker::is<ArrayType>(valueType))       return "[array]\n";  // or better: iterate and print separately
        if (TypeChecker::is<FileType>(valueType))        return "[file]\n";

        std::cerr << "[TypeChecker] Warning: Unrecognized type for format specifier\n";
        return "%p\n";
    }


    // Helper function to get or create the printf function
    llvm::Function* getOrCreatePrintfFunction(llvm::LLVMContext& context, llvm::Module* module) {
        llvm::FunctionType* printfType = llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context),                          // printf returns int
            {llvm::Type::getInt8PtrTy(context)},                      // first argument is a char pointer
            true                                                      // printf accepts variable arguments
        );
        // Insert or retrieve the printf function from the module
        auto printfFunctionCallee = module->getOrInsertFunction("printf", printfType);
        return llvm::cast<llvm::Function>(printfFunctionCallee.getCallee());
    }

    // Helper function to prepare arguments for the printf call
    std::vector<llvm::Value*> LoggerPrinterNode::preparePrintfArguments(std::shared_ptr<AstContext> astContext, llvm::Value* expressionValue) {
        auto& builder = astContext->getBuilder();
        auto* module = astContext->getModule();

        auto* llvmType = expressionValue->getType();
        std::vector<llvm::Value*> printfArgs;

        std::string formatSpecifier = getFormatSpecifier(llvmType);
        auto* formatString = builder.CreateGlobalStringPtr(formatSpecifier, "formatString");
        printfArgs.push_back(formatString);

        if(TypeChecker::is<BooleanType>(llvmType)) {
            std::cout << "Boolean print lllllll" << formatSpecifier << std::endl;
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
            auto charValue = builder.CreateExtractValue(expressionValue, {0}, "char_extract_val");
            printfArgs.push_back(charValue);
        } else {
            printfArgs.push_back(expressionValue);
        }

        return printfArgs;
    }

    llvm::Value* LoggerPrinterNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_INFO("IR Code Generation ...");

        auto* expressionValue = expressionNode->generateCode(astContext);
        auto* printfFunction = getOrCreatePrintfFunction(astContext->getLLVMContext(), astContext->getModule());
        auto printfArgs = preparePrintfArguments(astContext, expressionValue);

        auto& builder = astContext->getBuilder();
        return builder.CreateCall(printfFunction, printfArgs, "printfCall");
    }

    std::unique_ptr<Node> LoggerPrinterNode::clone() const {
        return std::make_unique<LoggerPrinterNode>(expressionNode ? expressionNode->clone() : nullptr);
    }

}
