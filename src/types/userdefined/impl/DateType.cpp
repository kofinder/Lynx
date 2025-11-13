#include "userdefined/DateType.hpp"
#include <context/AstContext.hpp>

using namespace LynxContext;

namespace LynxTypes {

    llvm::StructType* DateType::cachedType = nullptr;

    llvm::Type* DateType::computeLLVMType() const {
        LOG_INFO("Invoked...");

        if (!cachedType) {
            auto& context = astContext->getLLVMContext();
            cachedType = llvm::StructType::create(context, MetadataTypeConstants::structureDateType);
            std::vector<llvm::Type*> members = {
                llvm::Type::getInt32Ty(context), // year
                llvm::Type::getInt32Ty(context), // month
                llvm::Type::getInt32Ty(context), // day
                llvm::Type::getInt32Ty(context), // hour
                llvm::Type::getInt32Ty(context), // minute
                llvm::Type::getInt32Ty(context), // second
                llvm::Type::getInt32Ty(context)  // millisecond
            };
            cachedType = llvm::StructType::create(members, MetadataTypeConstants::structureDateType);
        }

        return cachedType;
    }

    llvm::Type* DateType::getLLVMPointerType() const {
        LOG_INFO("Invoked...");
        return llvm::PointerType::get(computeLLVMType()->getContext(), 0);
    }

    llvm::Value* DateType::getDefaultValue() {
        return llvm::Constant::getNullValue(computeLLVMType());
    }

    llvm::Value* DateType::createInstance(std::string variableName) {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getBuilder();
        llvm::Type* doubleType = this->getLLVMType();
        auto var = builder.CreateAlloca(doubleType, nullptr, variableName);
        if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(var)) {
            auto* metadata = llvm::MDNode::get(builder.getContext(), llvm::MDString::get(builder.getContext(), MetadataTypeConstants::dateType));
            var->setMetadata(MetadataTypeConstants::lynxDataType, metadata);
        }
        return var;
    }

    // llvm::Value* DateType::createValue(LValueType value) const {
    //     LOG_INFO("Invoked...");
        

    //     auto& builder = astContext->getBuilder();
    //     auto& context = astContext->getLLVMContext();
    
    //     const Date& date = std::get<Date>(value);
    //     auto* structType = llvm::cast<llvm::StructType>(computeLLVMType());

    //     llvm::Constant* constant = llvm::ConstantStruct::get(
    //         structType,
    //         {
    //             llvm::ConstantInt::get(context, llvm::APInt(32, date.year)),
    //             llvm::ConstantInt::get(context, llvm::APInt(32, date.month)),
    //             llvm::ConstantInt::get(context, llvm::APInt(32, date.day)),
    //             llvm::ConstantInt::get(context, llvm::APInt(32, 0)), // hour
    //             llvm::ConstantInt::get(context, llvm::APInt(32, 0)), // minute
    //             llvm::ConstantInt::get(context, llvm::APInt(32, 0)), // second
    //             llvm::ConstantInt::get(context, llvm::APInt(32, 0))  // millisecond
    //         });

    //     return constant;

    // }


    llvm::Value* DateType::assignTo(llvm::Value* lhs, llvm::Value* rhs) {
        LOG_INFO("Invoked...");
        if (!lhs || !rhs) {
            LOG_ERROR("Null pointer encountered during assignment: lhs or rhs is null.");
            return nullptr;
        }

        auto& builder = astContext->getBuilder();
        return builder.CreateStore(rhs, lhs);
    }

    llvm::Value* DateType::getField(std::string fieldName, llvm::Value* instance) {
        LOG_INFO("Invoked...");
        auto& context = astContext->getLLVMContext();
        auto& builder = astContext->getBuilder();
        llvm::Function* func = nullptr;
        return builder.CreateCall(func, { instance });    
    }

    bool DateType::equals(const BaseType* other) const {
        LOG_INFO("Invoked...");
        return dynamic_cast<const DateType*>(other) != nullptr;
    }

    const BaseType* DateType::createWithStatic(bool newIsStatic) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }

    const BaseType* DateType::createWithConst(bool newIsConst) const {
        LOG_INFO("Invoked...");
        return nullptr;
    }
    
    std::string DateType::getDebugName() const {
        LOG_INFO("Invoked...");
        return "Date";
    }

    llvm::DIType* DateType::getDIType(llvm::DIScope* scope) const {
        LOG_INFO("Invoked...");
        auto& builder = astContext->getDebugBuilder();

        llvm::DIFile* file = builder.createFile("date.lynx", ".");
        uint64_t sizeInBits = getDebugSizeInBits();
        uint32_t alignInBits = getDebugAlignInBits();

        std::vector<llvm::Metadata*> elements;

        llvm::DIType* int32Ty = builder.createBasicType("i32", 32, llvm::dwarf::DW_ATE_signed);

        std::vector<std::string> fieldNames = {
            "year", "month", "day", "hour", "minute", "second", "millisecond"
        };

        uint64_t offsetInBits = 0;
        for (const auto& name : fieldNames) {
            elements.push_back(builder.createMemberType(
                scope,
                name,
                file,
                0,                 // line number
                32,                // size in bits
                32,                // align in bits
                offsetInBits,      // offset in bits
                llvm::DINode::FlagZero,
                int32Ty
            ));
            offsetInBits += 32;
        }

        return builder.createStructType(
            scope,
            getDebugName(),       // Name: "Date"
            file,
            0,                    // line
            sizeInBits,
            alignInBits,
            llvm::DINode::FlagZero,
            nullptr,              // derived from
            builder.getOrCreateArray(elements)
        );
    }

    uint64_t DateType::getDebugSizeInBits() const {
        LOG_INFO("Invoked...");
        return 32 * 7;  // 7 fields * 32 bits
    }

    uint32_t DateType::getDebugAlignInBits() const {
        LOG_INFO("Invoked...");
        return 32; // 4-byte alignment
    }

    llvm::DINode::DIFlags DateType::getDIFlags() const {
        LOG_INFO("Invoked...");
        return llvm::DINode::FlagZero;
    }
}