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
        LOG_INFO("Invoked...");
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
        LOG_INFO("Invoked...");
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* DateTimeType::getDefaultValue() {
        LOG_INFO("Invoked...");
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
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* doubleType = this->getLLVMType();
        auto var = builder.CreateAlloca(doubleType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::dateTimeType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    llvm::Value* DateTimeType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    llvm::Value* DateTimeType::getField(std::string fieldName, llvm::Value* instance) {
        LOG_INFO("Invoked...");
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

    const BaseType* DateTimeType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* DateTimeType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    bool DateTimeType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const DateTimeType*>(other) != nullptr;
    }

    std::string DateTimeType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "DateTime";
    }

    llvm::DIType* DateTimeType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();

        llvm::DIFile* file = builder.createFile("datetime.lynx", ".");
        uint64_t sizeInBits = getDebugSizeInBits();
        uint32_t alignInBits = getDebugAlignInBits();

        std::vector<llvm::Metadata*> elements;

        llvm::DIType* int32Ty = builder.createBasicType("i32", 32, llvm::dwarf::DW_ATE_signed);

        std::vector<std::string> fieldNames = {
            "year", "month", "day", "hour", "minute", "second", "millisecond", "timezoneOffset"
        };

        uint64_t offsetInBits = 0;
        for (const auto& name : fieldNames) {
            elements.push_back(builder.createMemberType(
                scope,
                name,
                file,
                0,               // line
                32,              // size in bits
                32,              // align in bits
                offsetInBits,    // offset in bits
                llvm::DINode::FlagZero,
                int32Ty
            ));
            offsetInBits += 32;
        }

        return builder.createStructType(
            scope,
            getDebugName(),  // name
            file,
            0,               // line
            sizeInBits,
            alignInBits,
            llvm::DINode::FlagZero,
            nullptr,         // derived from
            builder.getOrCreateArray(elements)
        );
    }

    uint64_t DateTimeType::getDebugSizeInBits() const {
        return 8 * 32;
    }

    uint32_t DateTimeType::getDebugAlignInBits() const {
        return 32;
    }

    llvm::DINode::DIFlags DateTimeType::getDIFlags() const {
        return llvm::DINode::FlagZero;
    }

}