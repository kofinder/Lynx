#include "EnumDeclarationNode.hpp"
#include <logger/Logger.hpp>
#include <context/GlobalSymbolContext.hpp>
#include <types/userdefined/EnumType.hpp>
#include <constants/LinkageType.hpp>
#include <types/userdefined/helper/EnumMember.hpp>
        
namespace LynxAst {

    using namespace LynxLogger;
    using namespace LynxContext;
    using namespace LynxConstants;

    void EnumDeclarationNode::addMember(const std::string& memberName, const std::variant<int, char, std::string>& value) {
        members.emplace_back(memberName, value);
    }

    llvm::Value* EnumDeclarationNode::generateCode(std::shared_ptr<AstContext> astContext) {
        
        auto enumType = std::make_unique<EnumType>(astContext.get(), enumName);

        buildEnumType(*enumType);

        emitEnumConstants(*astContext, *enumType);
        
        astContext->registerCustomType(enumName, std::move(enumType));

        return nullptr;
    }

    void EnumDeclarationNode::buildEnumType(EnumType& enumType) const {
        int nextAutoValue = 0;

        for (const auto& [memberName, value] : members) {
            EnumMember member;

            if (std::holds_alternative<int>(value)) {
                int val = std::get<int>(value);
                if (val == -1) {
                    val = nextAutoValue;
                }
                member = EnumMember(memberName, val);
                member.setIndex(val);
                nextAutoValue = val + 1;
            } else if (std::holds_alternative<char>(value)) {
                member = EnumMember(memberName, std::get<char>(value));
                member.setIndex(nextAutoValue);
                nextAutoValue++;
            } else if (std::holds_alternative<std::string>(value)) {
                member = EnumMember(EnumName{memberName}, EnumValue{std::get<std::string>(value)});
                member.setIndex(nextAutoValue);
                nextAutoValue++;
            } else {
                std::cerr << "Unsupported enum value type...." << std::endl;
                throw std::runtime_error("Unsupported enum value type.");
            }

            enumType.addMember(memberName, member);
        }
    }

    std::vector<llvm::Constant*> EnumDeclarationNode::generatePayload(const AstContext& astContext, EnumMember member) const {
        auto* module = astContext.getModule();
        auto& context = astContext.getLLVMContext();

        std::string name = member.getName();

        llvm::Constant* intPayload = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), 0);
        llvm::Constant* charPayload = llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), 0);
        llvm::Constant* stringPayload = llvm::ConstantPointerNull::get(llvm::PointerType::get(context, 0));

        if(member.isCharValue()) {
            std::cerr << name << "::" << "is char value ::" << member.getIndex() << "\n";
            intPayload = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), member.getIndex());

            charPayload = llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), static_cast<uint8_t>(std::get<char>(member.getValue())));

            auto strValueConst = llvm::ConstantDataArray::getString(context, name, true);
            auto* strGlobal = new llvm::GlobalVariable(*module, strValueConst->getType(), true, privateType, strValueConst);
            stringPayload = llvm::ConstantExpr::getPointerCast(strGlobal, llvm::PointerType::get(context, 0));
        }

        if(member.isStringValue()) {
            std::cerr << name << "::" << "is string value ::" << member.getIndex() << "\n";
            intPayload = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), member.getIndex());

            char firstChar = !name.empty() ? static_cast<char>(std::toupper(name[0])) : 'X';
            charPayload = llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), static_cast<uint8_t>(firstChar));

            auto strValueConst = llvm::ConstantDataArray::getString(context, std::get<std::string>(member.getValue()), true);
            auto* strGlobal = new llvm::GlobalVariable(*module, strValueConst->getType(), true, privateType, strValueConst);
            stringPayload = llvm::ConstantExpr::getPointerCast(strGlobal, llvm::PointerType::get(context, 0));
        }

        if(member.isIntValue()) {
            std::cerr << name << "::" << "is int value ::" << member.getIndex() << "\n";
            intPayload = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), std::get<int>(member.getValue()));

            char firstChar = !name.empty() ? static_cast<char>(std::toupper(name[0])) : 'X';
            charPayload = llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), static_cast<uint8_t>(firstChar));

            auto strValueConst = llvm::ConstantDataArray::getString(context, name, true);
            auto* strGlobal = new llvm::GlobalVariable(*module, strValueConst->getType(), true, privateType, strValueConst);
            stringPayload = llvm::ConstantExpr::getPointerCast(strGlobal, llvm::PointerType::get(context, 0));
        }

        return {intPayload, charPayload, stringPayload};
    }

    void EnumDeclarationNode::emitEnumConstants(const AstContext& astContext, const EnumType& enumType) const {
        auto* module = astContext.getModule();
        auto& context = astContext.getLLVMContext();
        auto* llvmType = enumType.getLLVMType();
    
        int64_t tagCounter = 0;
    
        // Get the union type inside the enum type
        auto* unionType = llvm::cast<llvm::StructType>(enumType.createEnumValueUnion(context));
    
        for (const auto& [name, member] : enumType.getAllMembers()) {

            // Tag constant (discriminant)
            llvm::Constant* tagConst = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), tagCounter);

            auto payloadConstants = generatePayload(astContext, member);
            llvm::Constant* unionVal = llvm::ConstantStruct::get(unionType, payloadConstants);
    
            // Create the final enum struct constant: { tag, unionVal }
            llvm::Constant* structVal = llvm::ConstantStruct::get(llvm::cast<llvm::StructType>(llvmType), { tagConst, unionVal });
    
            // Name of the global constant, e.g. "enum.Direction.LEFT"
            std::string globalName = enumType.qualifiedName() + "." + name;
    
            // Create a global variable for the enum constant
            auto gv = new llvm::GlobalVariable(*module, llvmType, true, extType, structVal, globalName);
            tagCounter++;
        }
    }    

    std::unique_ptr<Node> EnumDeclarationNode::clone() const {
        return std::make_unique<EnumDeclarationNode>(*this); 
    }
}
