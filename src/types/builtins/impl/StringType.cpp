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
        return llvm::PointerType::get(llvm::Type::getInt8Ty(getContext()->getLLVMContext())->getContext(), 0);
    }

    llvm::Type* StringType::getLLVMPointerType() const {
        return llvm::PointerType::get(llvm::Type::getInt8Ty(getContext()->getLLVMContext())->getContext(), 0);
    }

    llvm::Value* StringType::getDefaultValue() {
        return llvm::Constant::getNullValue(computeLLVMType());
    }

    llvm::Value* StringType::createInstance(const std::string& variableName) {
        auto& builder = getContext()->getBuilder();
        llvm::Type* llvmType = getLLVMType();
        auto* var = builder.CreateAlloca(llvmType, nullptr, variableName);
        if(auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), stringType));
            var->setMetadata(lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* StringType::createValue(LValueType value) const {
        auto& context = getContext()->getLLVMContext();
        auto* module = getContext()->getModule();

        const llvm::StringRef strRef(std::get<std::string>(value));
        auto* strConst = llvm::ConstantDataArray::getString(context, strRef, true);

        auto* strArrayType = llvm::ArrayType::get(llvm::Type::getInt8Ty(context), strRef.size() + 1);
        auto* strVar = new llvm::GlobalVariable(*module, strArrayType, true, privateType, strConst, stringConstant);

        auto* stringType = computeLLVMType();

        return llvm::ConstantExpr::getBitCast(strVar, stringType);
    }

    llvm::Value* StringType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }
        auto& builder = getContext()->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    void StringType::accept(TypeVisitor& visitor) { visitor.visit(*this); }

    llvm::Value* StringType::emitMethodCall(llvm::Value* instance, llvm::Value* instancePtr, const std::string& methodName, const std::vector<llvm::Value*>& args) {
        if (!resolver) resolver = getOrCreateResolver();
        return resolver->resolveMethod(*getContext(), instance, instancePtr, methodName, args);
    }

    TypeMethodResolver* StringType::getOrCreateResolver() const { 
        return nullptr;
    }

    bool StringType::equals(const BaseType* other) const {
        return dynamic_cast<const StringType*>(other) != nullptr;
    }

    const BaseType* StringType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* StringType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* StringType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t StringType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t StringType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags StringType::getDIFlags() const { return llvm::DINode::FlagZero; }
}
