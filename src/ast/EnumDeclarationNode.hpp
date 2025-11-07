/**
 * @file EnumDeclarationNode.hpp
 * @brief Declares the EnumDeclarationNode class representing enum declarations in the Lynx AST.
 * 
 * The EnumDeclarationNode class manages the definition of enums, including their name, members,
 * and compile-time constant values. It integrates with the AST for code generation and semantic checks.
 * 
 * **Key Responsibilities:**
 * - Stores enum name and members with associated literal values (int, char, or string).
 * - Builds EnumType representation for the compiler type system.
 * - Generates LLVM constants for enum members.
 * - Provides deep cloning for AST transformations.
 * 
 * **Used By:**
 * - Enum type management within the compiler.
 * - Code generation and constant evaluation for enums.
 * 
 * @see Node, EnumType, EnumMember
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_ENUM_DECLARATION_NODE_HPP
#define LYNX_ENUM_DECLARATION_NODE_HPP

#include "Node.hpp"
#include <types/userdefined/EnumType.hpp>

namespace LynxAst {

    using namespace LynxTypes;
    using namespace LynxConstants;

    class EnumDeclarationNode : public Node {

        protected:

            std::string enumName;

            std::vector<std::pair<std::string, std::variant<int, char, std::string>>> members;

        private:

            void buildEnumType(EnumType& enumType) const;

            void emitEnumConstants(const AstContext& astContext, const EnumType& enumType) const;

            std::vector<llvm::Constant*> generatePayload(const AstContext& astContext, EnumMember member) const;

        public:

            explicit EnumDeclarationNode(std::string name): enumName(std::move(name)) {}

            EnumDeclarationNode(
                std::string name, 
                std::vector<std::pair<std::string, std::variant<int, char, std::string>>> memberList
            ): enumName(std::move(name)), members(std::move(memberList)) {}

            std::unique_ptr<Node> clone() const override;
            
            inline constexpr NodeType getNodeType() override { return NodeType::ENUM_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            const std::string getEnumName() const { return enumName; }

            void addMember(const std::string& name, const std::variant<int, char, std::string>& value);

            [[nodiscard]] inline std::vector<std::pair<std::string, std::variant<int, char, std::string>>> getMembers() const noexcept { return members; }

            ~EnumDeclarationNode() override = default;
    };
}

#endif
