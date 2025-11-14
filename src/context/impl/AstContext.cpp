#include "AstContext.hpp"
#include "GlobalSymbolContext.hpp"
#include <llvm/IR/Verifier.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>

#include <types/builtins/ByteType.hpp>
#include <types/builtins/ShortType.hpp>
#include <types/builtins/IntegerType.hpp>
#include <types/builtins/LongType.hpp>
#include <types/builtins/FloatType.hpp>
#include <types/builtins/DoubleType.hpp>
#include <types/builtins/CharType.hpp>
#include <types/builtins/BooleanType.hpp>
#include <types/builtins/StringType.hpp>
#include <types/builtins/VoidType.hpp>
#include <types/associative/MapType.hpp>
#include <types/associative/DictionaryType.hpp>

#include <types/sequential/ArrayType.hpp>
#include <types/sequential/VectorType.hpp>
#include <types/sequential/ListType.hpp>
#include <types/sequential/QueueType.hpp>
#include <types/sequential/SetType.hpp>
#include <types/sequential/StackType.hpp>

#include <types/userdefined/EnumType.hpp>
#include <types/userdefined/DateType.hpp>
#include <types/userdefined/DateTimeType.hpp>
#include <types/userdefined/FileType.hpp>
#include <types/infrerence/AutoType.hpp>

#include <logger/Logger.hpp>
#include <errors/LoggingVisitor.hpp>
#include <types/visitor/TypeMethodCallVisitor.hpp>

namespace LynxContext {

    using namespace LynxTypes;
    using namespace LynxLogger;
    using namespace LynxConstants;


    AstContext::AstContext(
        const std::string& moduleName, 
        std::shared_ptr<llvm::LLVMContext> ctx, 
        std::shared_ptr<GlobalSymbolContext> symbol,
        std::shared_ptr<std::map<std::string, std::shared_ptr<BaseType>>> sharedTypes
    ) : llvmContext(std::move(ctx)),
        globalContext(std::move(symbol)),
        module(std::make_shared<llvm::Module>(moduleName, *llvmContext)),
        builder(std::make_shared<llvm::IRBuilder<>>(*llvmContext)),
        debugBuilder(std::make_shared<llvm::DIBuilder>(*module)),
        errors(std::make_shared<CompositeError>()),
        methodRegistry(std::make_shared<TypeMethodRegistry>()),
        dataLayout(module->getDataLayout()),
        types(sharedTypes ? sharedTypes : std::make_shared<std::map<std::string, std::shared_ptr<BaseType>>>()) {}

    std::shared_ptr<AstContext> AstContext::createContext() const {          
        auto newCtx = std::make_shared<AstContext>(
            module->getName().str(), 
            llvmContext, 
            globalContext, 
            types
        );

        newCtx->types = this->types;
        newCtx->errors = this->errors;
        newCtx->builder = this->builder;
        newCtx->module = this->module;
        newCtx->dataLayout = this->dataLayout;
        newCtx->debugBuilder = this->debugBuilder;
        newCtx->globalContext = this->globalContext;
        newCtx->methodRegistry = this->methodRegistry;
        newCtx->currentDepth = this->currentDepth + 1;

        // Rebind types to the new context
        for (auto& [name, typePtr] : *newCtx->types) {
            if (typePtr && typePtr->getContext() != newCtx.get()) {
                typePtr->setContext(newCtx.get());
            }
        }    
        
        return newCtx;  
    }

    void AstContext::initializeDefaultTypes() {
        // Core primitive types
        registerCustomType("byte", std::make_shared<ByteType>(this));
        registerCustomType("short", std::make_shared<ShortType>(this));
        registerCustomType("int", std::make_shared<IntegerType>(this));
        registerCustomType("long", std::make_shared<LongType>(this));
        registerCustomType("float", std::make_shared<FloatType>(this));
        registerCustomType("double", std::make_shared<DoubleType>(this));
        registerCustomType("char", std::make_shared<CharType>(this));
        registerCustomType("boolean", std::make_shared<BooleanType>(this));
        registerCustomType("string", std::make_shared<StringType>(this));
        registerCustomType("void", std::make_shared<VoidType>(this));

        // Extended types
        registerCustomType("Date", std::make_shared<DateType>(this));
        registerCustomType("File", std::make_shared<FileType>(this));
        registerCustomType("auto", std::make_shared<AutoType>(this));
        registerCustomType("DateTime", std::make_shared<DateTimeType>(this));

        // Collection types
        registerCustomType("array", std::make_shared<ArrayType>(this));
        registerCustomType("vector", std::make_shared<VectorType>(this));
        registerCustomType("list", std::make_shared<ListType>(this));
        registerCustomType("set", std::make_shared<SetType>(this));
        registerCustomType("map", std::make_shared<MapType>(this));
        registerCustomType("stack", std::make_shared<StackType>(this));
        registerCustomType("queue", std::make_shared<QueueType>(this));
        registerCustomType("dictionary", std::make_shared<DictionaryType>(this));
    } 

    void AstContext::initializeTypeMethods() {
        for (auto& [typeName, typePtr] : *types) {
            const auto& methods = typePtr->getMethodRegistry();
            if (!methods.empty()) methodRegistry->registerMethods(typeName, methods);
        }
    }

    std::shared_ptr<BaseType> AstContext::findType(DataType dt) const {
        return findType(dataTypeToString(dt));  
    }

    std::shared_ptr<BaseType> AstContext::findType(llvm::Value* llvmValue) const {
        if (!llvmValue) return nullptr;
        llvm::Type* valType = llvmValue->getType();
        for (const auto& [name, typePtr] : *types) {
            if (!typePtr || typePtr->getTypeTag() == DataType::AUTO) continue;
            if (typePtr->getLLVMType() == valType) return typePtr;
        }

        std::cerr << "[AstContext] Type not found! \n";
        return nullptr;
    }
    
    std::shared_ptr<BaseType> AstContext::findType(const std::string& name) const {
        auto it = types->find(name);
        return it != types->end() ? it->second : nullptr;
    }

    std::shared_ptr<BaseType> AstContext::findType(VariableType varType) const {
        if(varType.type == DataType::OTHER) {
            return this->findType(varType.name);
        }
        return this->findType(varType.type);
    }
        
    bool AstContext::registerCustomType(const std::string& name, std::shared_ptr<BaseType> type) {
        if(this->findType(name) != nullptr) {
            return true;
        }
        types->emplace(name, type);
        return true;
    }

    llvm::Value* AstContext::emitGCAllocCall(llvm::Type* objType, std::string objectName) {

        // Compute allocation size
        uint64_t typeSize = getDataLayout().getTypeAllocSize(objType);
        auto* allocSize = llvm::ConstantInt::get(llvm::Type::getInt64Ty(getLLVMContext()), typeSize);

        // Get or insert the GC allocation function
        auto* mallocFn = getOrInsertGCAllocFunc(allocSize, "LYNX_GC_ALLOC_" + objectName);

        // Emit the call in IR
        auto& builder = getBuilder();
        auto* mallocCall = builder.CreateCall(mallocFn, { allocSize }, "gc_alloc");

        // Cast to the correct object pointer type
        auto* objectPtrTy = llvm::PointerType::get(objType->getContext(), 0);
        auto objectInstance = builder.CreateBitCast(mallocCall, objectPtrTy, "gc_cast");

        auto baseType = findType(objectInstance);
        if(auto clazzType = TypeCasting::castType<ClassType>(baseType.get())) {
            LOG_INFO("Binding Vtable to the {}", clazzType->qualifiedName());
            clazzType->bindVTable(objectInstance);
        }

        return objectInstance;
    }

    llvm::Function* AstContext::getOrInsertGCAllocFunc(llvm::ConstantInt* allocSize, const std::string& fnName) {
    
        auto* voidPtrType = llvm::PointerType::get(getLLVMContext(), 0);

        // Define function type: i8* func(i64)
        auto mallocType = llvm::FunctionType::get(voidPtrType, { allocSize->getType() }, false);

        // Retrieve or create function in the module
        auto mallocCallee = getModule()->getOrInsertFunction(fnName, mallocType);

        llvm::Function* mallocFn = nullptr;
        if (auto* func = llvm::dyn_cast<llvm::Function>(mallocCallee.getCallee())) {
            mallocFn = func;
        } else {
            mallocFn = llvm::cast<llvm::Function>(mallocCallee.getCallee()->stripPointerCasts());
        }

        return mallocFn;
    }

    std::shared_ptr<GlobalSymbolContext> AstContext::getGlobalContext() const { return globalContext; }

    void AstContext::reportError(std::shared_ptr<LynxError> err) { errors->add(err); }

    void AstContext::logErrors() {
        LoggingVisitor logger;
        errors->accept(logger);
    }

}
