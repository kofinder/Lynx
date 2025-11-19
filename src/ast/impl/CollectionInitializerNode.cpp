#include "LiteralNode.hpp"
#include "ExpressionNode.hpp"
#include "PrimaryExpressionNode.hpp"
#include "CollectionInitializerNode.hpp"
#include <types/tmpl/TypeCaster.hpp>


namespace LynxAst {

    llvm::Value* CollectionInitializerNode::generateCode(std::shared_ptr<AstContext> astContext) {
        LOG_WARN("IR Code Generation.");
        auto& builder = astContext->getBuilder();
        auto variableType = nullptr;
        llvm::Value* result = generateLLVMFromCollection(this, variableType, astContext);

        std::cout << "\n \n ******* End Printer Collection Initilizer LLVM **** \n" << std::endl;

        return result;
    }

    llvm::Value* CollectionInitializerNode::generateLLVMFromCollection(
        Node* node,
        std::shared_ptr<VariableType> varType,
        std::shared_ptr<AstContext> astContext
    ) {
        if (auto literalNode = dynamic_cast<LiteralNode*>(node)) {
            if (!literalNode) {
                throw std::runtime_error("Non-literal node passed to extractLiteralValue");
            }            
            return extractLiteralValue(literalNode, varType->type, *astContext);
        }
    
        if (auto collectionNode = dynamic_cast<CollectionInitializerNode*>(node)) {
            const CollectionType* baseType = getCollectionType(*varType, *astContext);            
            resolveInternalTypes(varType, *astContext);

            if (!baseType) {
                // std::cerr << "Invalid collection type: " << toString(varType->type) << "\n";
                astContext->reportError(makeRuntimeError("Invalid collection type"));
                return nullptr;
            }
    
            if (collectionNode->isList()) {
                auto elementType = varType->getFirstGeneric();
                auto& list = std::get<std::unique_ptr<LiteralListNode>>(collectionNode->getValue())->getValues();
                std::vector<llvm::Value*> values;    
                for (const auto& elem : *list) {
                    llvm::Value* val = generateLLVMFromCollection(elem.get(), elementType, astContext);
                    values.push_back(val);
                }
                auto* concreteCollection = const_cast<CollectionType*>(baseType);
                concreteCollection->setElementType(elementType->getInternalType());
                concreteCollection->setSize(values.size());
                //concreteCollection->createInstance(mangleSequentialName(concreteCollection, values));

                
                return concreteCollection->createValue(values);
            }
    
            if (collectionNode->isMap()) {
                auto& map = std::get<std::unique_ptr<LiteralMapNode>>(collectionNode->getValue())->getPairs();
                std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs;
                auto keyElementType = varType->getFirstGeneric();
                auto valueElementType = varType->getSecondGeneric();
                for (const auto& pair : *map) {
                    llvm::Value* keyVal = generateLLVMFromCollection(pair->getKey(), keyElementType, astContext);
                    llvm::Value* val = generateLLVMFromCollection(pair->getValue(), valueElementType, astContext);
                    pairs.emplace_back(keyVal, val);
                }

                auto* concreteCollection = const_cast<CollectionType*>(baseType);
                concreteCollection->setElementType(keyElementType->getInternalType());
                concreteCollection->setValueType(valueElementType->getInternalType());
                concreteCollection->setSize(0);
                concreteCollection->createValue(pairs);
                return concreteCollection->createInstance(mangleAssociativeName(concreteCollection, pairs));
            }
        }
    
        astContext->reportError(makeRuntimeError("Invalid node in collection initializer"));
        return nullptr;
    }


    const CollectionType* CollectionInitializerNode::getCollectionType(const VariableType& varType, const AstContext& astContext) const noexcept {
        auto dataType = varType.type;
        auto collectionType = astContext.findType(dataType);
        auto* baseType = collectionType.get();
        switch (dataType) {
            case DataType::ARRAY: return TypeCasting::castType<ArrayType>(baseType);
            case DataType::VECTOR: return TypeCasting::castType<VectorType>(baseType);
            case DataType::LIST: return TypeCasting::castType<ListType>(baseType);
            case DataType::SET: return TypeCasting::castType<SetType>(baseType);
            case DataType::MAP: return TypeCasting::castType<MapType>(baseType);
            case DataType::STACK: return TypeCasting::castType<StackType>(baseType);
            case DataType::QUEUE: return TypeCasting::castType<QueueType>(baseType);
            case DataType::DICT: return TypeCasting::castType<DictionaryType>(baseType);
            default:
                std::cerr << "Unknown collection type: " << static_cast<int>(dataType) << "\n";
                return nullptr;
        }
    }

    llvm::Value* CollectionInitializerNode::extractLiteralValue(const Node* node,  DataType dataType, const AstContext& astContext) const {
        if (auto literalNode = dynamic_cast<const LiteralNode*>(node)) {
            if (!literalNode) {
                throw std::runtime_error("Non-literal node passed to extractLiteralValue");
            }
        
            auto& builder = astContext.getBuilder();
            auto& context = astContext.getLLVMContext();
            if(dataType == DataType::BYTE) {
                auto value = std::get<uint8_t>(literalNode->getLiteralValue());
                return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), value);
            } else if(dataType == DataType::SHORT) {
                auto value = std::get<short>(literalNode->getLiteralValue());
                return llvm::ConstantInt::get(llvm::Type::getInt16Ty(context), value);
            } else if(dataType == DataType::INT) {
                auto value = std::get<int>(literalNode->getLiteralValue());
                return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), value);
            } else if(dataType == DataType::LONG) {
                auto value = std::get<long>(literalNode->getLiteralValue());
                return llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), value);
            } else if(dataType == DataType::FLOAT) {
                auto value = std::get<float>(literalNode->getLiteralValue());
                return llvm::ConstantFP::get(llvm::Type::getFloatTy(context), value);
            } else if(dataType == DataType::DOUBLE) {
                auto value = std::get<double>(literalNode->getLiteralValue());
                return llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), value);
            } else if(dataType == DataType::CHAR) {
                auto value = std::get<char>(literalNode->getLiteralValue());
                return llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), value);
            } else if(dataType == DataType::BOOLEAN) {
                auto value = std::get<bool>(literalNode->getLiteralValue());
                return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), value);
            } else if(dataType == DataType::STRING) {
                auto value = std::get<std::string>(literalNode->getLiteralValue());
                llvm::StringRef strRef(value);
                return llvm::ConstantDataArray::getString(context, strRef, true);
            } else {
                std::cerr << "Non-literal node passed to extractLiteralValue \n";
                return nullptr;
            }
        }
        throw std::runtime_error("Non-literal node passed to extractLiteralValue");    
    }

    bool CollectionInitializerNode::isNestedListUniform(const std::vector<std::unique_ptr<Node>>& values) const noexcept {
        if (values.empty()) return true;
        auto firstNested = dynamic_cast<const CollectionInitializerNode*>(values[0].get());
        if (!firstNested) return false;
        size_t expectedSize = firstNested->getElementsCount();
        for (const auto& v : values) {
            auto nested = dynamic_cast<const CollectionInitializerNode*>(v.get());
            if (!nested || nested->getElementsCount() != expectedSize) return false;
        }
        return true;    
    }

    std::string CollectionInitializerNode::mangleSequentialName(const CollectionType* collectionType, const std::vector<llvm::Value*>& values) const noexcept {
        std::stringstream ss;
        std::string prefix;
        switch (collectionType->getTypeTag()) {
            case DataType::ARRAY: prefix = "arr_"; break;
            case DataType::VECTOR: prefix = "vec_"; break;
            case DataType::LIST: prefix = "lst_"; break;
            case DataType::SET: prefix = "set_"; break;
            case DataType::STACK: prefix = "stk_"; break;
            case DataType::QUEUE: prefix = "que_"; break;
            default: prefix = "col_"; break; // fallback
        }
    
        // ss << prefix << toString(collectionType->getElementType()->getTypeTag());
        ss << "_" << values.size();
        return ss.str();   
    }
    
    std::string CollectionInitializerNode::mangleAssociativeName(const CollectionType* collectionType, const std::vector<std::pair<llvm::Value*, llvm::Value*>>& pairs) const noexcept {
        std::stringstream ss;

        std::string prefix;
        switch (collectionType->getTypeTag()) {
            case DataType::MAP: prefix = "map_"; break;
            case DataType::DICT: prefix = "dict_"; break;
            default: prefix = "assoc_"; break;
        }
    
        ss << prefix
        //    << toString(collectionType->getElementType()->getTypeTag())
           << "_to_"
        //    << toString(collectionType->getElementType()->getTypeTag())
           << "_" << pairs.size();
    
        return ss.str();    
    }    


    size_t CollectionInitializerNode::getElementsCount() const noexcept {
        if (isList()) {
            const auto& listNode = std::get<std::unique_ptr<LiteralListNode>>(value);
            if (!listNode) return 0;
            return listNode->getValues()->size();
        } else {
            const auto& mapNode = std::get<std::unique_ptr<LiteralMapNode>>(value);
            if (!mapNode) return 0;
            return mapNode->getPairs()->size();
        }
    }

    void CollectionInitializerNode::resolveInternalTypes(const std::shared_ptr<VariableType>& varType, const AstContext& context) const noexcept {

        // Recursively resolve internal types of generic arguments
        for (auto& generic : varType->genericArguments) {
            resolveInternalTypes(generic, context);
        }

        auto base = context.findType(varType->type);
        if (!base) {
            // std::cerr << "Error: No base type found for " << toString(varType->type) << std::endl;
            return;
        }

        BaseType* raw = base.get();
        varType->internalType = raw;  // Default assignment

        // Specialize based on DataType
        switch (varType->type) {
            case DataType::ARRAY:
                varType->internalType = TypeCasting::castType<ArrayType>(raw);
                break;
            case DataType::VECTOR:
                varType->internalType = TypeCasting::castType<VectorType>(raw);
                break;
            case DataType::LIST:
                varType->internalType = TypeCasting::castType<ListType>(raw);
                break;
            case DataType::SET:
                varType->internalType = TypeCasting::castType<SetType>(raw);
                break;
            case DataType::MAP:
                varType->internalType = TypeCasting::castType<MapType>(raw);
                break;
            case DataType::STACK:
                varType->internalType = TypeCasting::castType<StackType>(raw);
                break;
            case DataType::QUEUE:
                varType->internalType = TypeCasting::castType<QueueType>(raw);
                break;
            case DataType::DICT:
                varType->internalType = TypeCasting::castType<DictionaryType>(raw);
                break;
            default:
                break;
        }
    }

    std::unique_ptr<Node> CollectionInitializerNode::clone() const {
        return std::visit([](const auto& ptr) -> std::unique_ptr<Node> {
            using T = std::decay_t<decltype(ptr)>;
            return std::make_unique<CollectionInitializerNode>(
                std::unique_ptr<typename T::element_type>(
                    static_cast<typename T::element_type*>(ptr->clone().release())
                )
            );
        }, value);
    }
}

