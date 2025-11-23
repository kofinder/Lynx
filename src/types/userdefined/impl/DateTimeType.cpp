#include "userdefined/DateTimeType.hpp"
#include <context/AstContext.hpp>
#include <logger/Logger.hpp>
#include "visitor/TypeVisitor.hpp"
#include "resolver/TypeMethodResolver.hpp"
#include "resolver/methods/DateTimeMethodResolver.hpp"

namespace LynxTypes { 

    using namespace LynxContext;
    using namespace LynxLogger;
    
    enum class DateTimeFieldIndex : std::uint8_t {
        Year = 0,
        Month,
        Day,
        Hour,
        Minute,
        Second,
        Millisecond,
        TimezoneOffset
    };    

    llvm::StructType* DateTimeType::cachedType = nullptr;

    llvm::Type* DateTimeType::computeLLVMType() const {
        if (!cachedType) {
            auto& context = getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::structureDateTimeType);
            const std::vector<llvm::Type*> members = {
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

    llvm::Value* DateTimeType::createInstance(const std::string& variableName) {
        auto& builder = getBuilder();
        auto* var = builder.CreateAlloca(getLLVMType(), nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::dateTimeType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* DateTimeType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        return getBuilder().CreateStore(rhs, lhs);
    }

    llvm::Value* DateTimeType::getField(const std::string& fieldName, llvm::Value* instance) {
        static const std::unordered_map<std::string, DateTimeFieldIndex> fieldIndex = {
            {"year", DateTimeFieldIndex::Year},
            {"month", DateTimeFieldIndex::Month},
            {"day", DateTimeFieldIndex::Day},
            {"hour", DateTimeFieldIndex::Hour},
            {"minute", DateTimeFieldIndex::Minute},
            {"second", DateTimeFieldIndex::Second},
            {"millisecond", DateTimeFieldIndex::Millisecond},
            {"timezoneOffset", DateTimeFieldIndex::TimezoneOffset}
        };
    
        auto iter = fieldIndex.find(fieldName);
        if (iter == fieldIndex.end()) {
            LOG_ERROR("Unknown field name in DateTimeType: ", fieldName);
            return nullptr;
        }    
        
        auto& builder = getBuilder();
        const auto index = static_cast<unsigned>(iter->second);
        auto* gep = builder.CreateStructGEP(computeLLVMType(), instance, index);
        return builder.CreateLoad(llvm::Type::getInt32Ty(getLLVMContext()), gep);
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