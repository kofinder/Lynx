#ifndef LYNX_ARRAY_INITIALIZER_NODE_HPP
#define LYNX_ARRAY_INITIALIZER_NODE_HPP


#include "Node.hpp"
#include <optional>
#include <constants/DataType.hpp>

using namespace LynxConstants;

namespace LynxAst {
    
    class ArrayInitializerNode: public Node {

        private:
            using ArrayVariant = std::variant<
                std::vector<uint8_t>, std::vector<short>, std::vector<int>, std::vector<long>,
                std::vector<float>, std::vector<double>, std::vector<char>, std::vector<bool>, std::vector<std::string>
            >;

            using MultiDimArrayVariant = std::variant<
                std::vector<std::vector<uint8_t>>, std::vector<std::vector<short>>, std::vector<std::vector<int>>, std::vector<std::vector<long>>,
                std::vector<std::vector<float>>, std::vector<std::vector<double>>, std::vector<std::vector<char>>, std::vector<std::vector<bool>>, 
                std::vector<std::vector<std::string>>
            >;

            std::optional<ArrayVariant> singleDimArray;

            std::optional<MultiDimArrayVariant> multiDimArray;

            DataType arrayType;

            llvm::Value* generateBooleanArray(std::shared_ptr<AstContext> astContext);

            llvm::Value* generateByteArray(std::shared_ptr<AstContext> astContext);

            llvm::Value* generateShortArray(std::shared_ptr<AstContext> astContext);

            llvm::Value* generateIntegerArray(std::shared_ptr<AstContext> astContext);

            llvm::Value* generateLongArray(std::shared_ptr<AstContext> astContext);

            llvm::Value* generateFloatArray(std::shared_ptr<AstContext> astContext);

            llvm::Value* generateDoubleArray(std::shared_ptr<AstContext> astContext);

            llvm::Value* generateStringArray(std::shared_ptr<AstContext> astContext);

            llvm::Value* generateCharArray(std::shared_ptr<AstContext> astContext);

        public:

            template <typename T>
            explicit ArrayInitializerNode(std::vector<T> values, DataType dataType) {
                singleDimArray = ArrayVariant(values);
                arrayType = dataType;
            }

            template <typename T>
            explicit ArrayInitializerNode(std::vector<std::vector<T>> values, DataType dataType) {
                multiDimArray = MultiDimArrayVariant(values);
                arrayType = dataType;
            }

            std::unique_ptr<Node> clone() const override {
                return std::make_unique<ArrayInitializerNode>(*this); 
            }

            NodeType getNodeType() override { return NodeType::ARRAY_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            inline size_t arraySize() const {
                if (singleDimArray) {
                    return std::visit([](auto&& arg) { 
                        return arg.size(); 
                    }, *singleDimArray);
                } else if (multiDimArray) {
                    return std::visit([](auto&& arg) {
                        size_t totalSize = 0;
                        for (const auto& row : arg) {
                            totalSize += row.size();
                        }
                        return totalSize;
                    }, *multiDimArray);
                }
                return 0; 
            }

            template <typename T>
            llvm::GlobalVariable* generateGlobalArray(llvm::Module* module, llvm::Type* elementType, const std::vector<llvm::Constant*>& arrayValues, const std::string& arrayName, unsigned alignment) {
                llvm::ArrayType* arrayType = llvm::ArrayType::get(elementType, arrayValues.size());
                llvm::Constant* arrayConst = llvm::ConstantArray::get(arrayType, arrayValues);
                llvm::GlobalVariable* globalArray = new llvm::GlobalVariable(*module, arrayType, true, llvm::GlobalValue::PrivateLinkage, arrayConst, arrayName);
                globalArray->setAlignment(llvm::Align(alignment));
                return globalArray;
            }        

            ~ArrayInitializerNode() override {}

    };
}

#endif 
