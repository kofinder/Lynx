#include <string>
#include "builtins/StringType.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/Support/raw_ostream.h>
#include <constants/LinkageType.hpp>
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/StringMethodResolver.hpp"

namespace LynxTypes {

    llvm::Type* StringType::computeLLVMType() const {
        return llvm::PointerType::get(llvm::Type::getInt8Ty(astContext->getLLVMContext())->getContext(), 0);
    }

    llvm::Type* StringType::getLLVMPointerType() const {
        return llvm::PointerType::get(llvm::Type::getInt8Ty(astContext->getLLVMContext())->getContext(), 0);
    }

    llvm::Value* StringType::getDefaultValue() {
        return llvm::Constant::getNullValue(computeLLVMType());
    }

    llvm::Value* StringType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* llvmType = this->getLLVMType();
        auto var = builder.CreateAlloca(llvmType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), stringType));
            var->setMetadata(lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* StringType::createValue(LValueType value) const {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        auto* module = astContext->getModule();

        llvm::StringRef strRef(std::get<std::string>(value));
        auto* strConst = llvm::ConstantDataArray::getString(context, strRef, true);

        auto* strArrayType = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), strRef.size() + 1);
        auto* strVar = new llvm::GlobalVariable(*module, strArrayType, true, privateType, strConst, stringConstant);

        auto* stringType = computeLLVMType();

        return llvm::ConstantExpr::getBitCast(strVar, stringType);
    }

    llvm::Value* StringType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    void StringType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    llvm::Value* StringType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        LOG_ERROR("Emit Method Call Invocation.");
        if (!resolver) resolver = getOrCreateResolver();
        return resolver->resolveMethod(*astContext, instance, instancePtr, methodName, args);
    }

    TypeMethodResolver* StringType::getOrCreateResolver() const { 
        if (!resolver) resolver = new StringMethodResolver();
        return resolver;
    }

    bool StringType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const StringType*>(other) != nullptr;
    }

    const BaseType* StringType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* StringType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    std::string StringType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "string";
    }

    llvm::DIType* StringType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();

        llvm::DIType* pointeeType = builder.createBasicType(
            "char",
            8,
            llvm::dwarf::DW_ATE_signed_char
        );

        return builder.createPointerType(
            pointeeType,
            getDebugSizeInBits(),   // 64 on 64-bit platforms
            getDebugAlignInBits()
        );
    }

    uint64_t StringType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 64; // Pointer size (e.g., 64-bit for x86_64)
    }

    uint32_t StringType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 64; // Alignment for pointers on 64-bit architectures
    }

    llvm::DINode::DIFlags StringType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}
