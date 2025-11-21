#include "userdefined/DateTimeType.hpp"
#include <context/AstContext.hpp>
#include <logger/Logger.hpp>
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/DateTimeMethodResolver.hpp"

using namespace LynxContext;
using namespace LynxLogger;

namespace LynxTypes {

    llvm::StructType* DateTimeType::cachedType = nullptr;

    llvm::Type* DateTimeType::computeLLVMType() const {
        if (!cachedType) {
            auto& context = astContext->getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::structureDateTimeType);
            std::vector<llvm::Type*> members = {
                llvm::Type::getInt32Ty(context), // year
                llvm::Type::getInt32Ty(context), // month
                llvm::Type::getInt32Ty(context), // day
                llvm::Type::getInt32Ty(context), // hour
                llvm::Type::getInt32Ty(context), // minute
                llvm::Type::getInt32Ty(context), // second
                llvm::Type::getInt32Ty(context),  // millisecond
                llvm::Type::getInt32Ty(context)  // timezone offset in minutes (e.g., -300 for UTC-5)
            };
            cachedType->setBody(members);
        }
    
        return cachedType;
    }

    llvm::Type* DateTimeType::getLLVMPointerType() const {
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* DateTimeType::getDefaultValue() {
        return llvm::Constant::getNullValue(computeLLVMType());
    }

    // void DateTimeType::accept(TypeVisitor& visitor) { 
    //     LOG_INFO("Invoked...");
    //     visitor.visit(*this); 
    // }

    // std::unique_ptr<TypeMethodResolver> DateTimeType::getOrCreateResolver() const {
    //     LOG_INFO("Invoked...");
    //     return std::make_unique<DateTimeMethodResolver>();
    // }


    llvm::Value* DateTimeType::createInstance(std::string variableName) {
        auto& builder = astContext->getBuilder();
        llvm::Type* doubleType = this->getLLVMType();
        auto* var = builder.CreateAlloca(doubleType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::dateTimeType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* DateTimeType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        if (!isValid(lhs) || !isValid(rhs)) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    llvm::Value* DateTimeType::getField(std::string fieldName, llvm::Value* instance) {
        auto& builder = astContext->getBuilder();

        static std::unordered_map<std::string, unsigned> fieldIndex = {
            {"year", 0}, {"month", 1}, {"day", 2}, {"hour", 3},
            {"minute", 4}, {"second", 5}, {"millisecond", 6}, {"timezoneOffset", 7}
        };
    
        auto it = fieldIndex.find(fieldName);
        if (it == fieldIndex.end()) {
            LOG_ERROR("Unknown field name in DateTimeType: ", fieldName);
            return nullptr;
        }
    
        llvm::Value* gep = builder.CreateStructGEP(computeLLVMType(), instance, it->second);
        return builder.CreateLoad(llvm::Type::getInt32Ty(astContext->getLLVMContext()), gep);
    }

    bool DateTimeType::equals(const BaseType* other) const {
        return dynamic_cast<const DateTimeType*>(other) != nullptr;
    }

    const BaseType* DateTimeType::createWithStatic(bool /*newIsStatic*/) const { return nullptr; }
    const BaseType* DateTimeType::createWithConst(bool /*newIsConst*/) const { return nullptr; }

    llvm::DIType* DateTimeType::getDIType(llvm::DIScope* /*scope*/) const { return nullptr;  }
    uint64_t DateTimeType::getDebugSizeInBits() const { return DEFAULT_ALIGN_BITS; }
    uint32_t DateTimeType::getDebugAlignInBits() const { return DEFAULT_ALIGN_BITS; }
    llvm::DINode::DIFlags DateTimeType::getDIFlags() const { return llvm::DINode::FlagZero; }
}