/**
 * @file MethodCallNode.hpp
 * @brief Declares the MethodCallNode class, representing method calls on objects in the Lynx AST.
 * 
 * The MethodCallNode class models calls to methods on objects, handling both built-in/library types
 * and user-defined types (classes, interfaces, and mixins). It supports LLVM IR code generation, null-safe
 * dispatch, and runtime or compile-time method resolution.
 * 
 * **Key Responsibilities:**
 * - Encapsulates the function call node and target object node.
 * - Classifies the method call target (built-in, library, or user-defined type).
 * - Dispatches methods with correct LLVM IR generation, including virtual method handling.
 * - Supports null-safe calls and chained method calls.
 * - Provides deep cloning of the node and its components.
 * 
 * **Used By:**
 * - AST construction and semantic analysis subsystems.
 * - LLVM IR generation for object-oriented method calls.
 * 
 * @see FunctionCallNode, ChainedDereferenceNode
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/


#ifndef LYNX_METHOD_CALL_NODE_HPP
#define LYNX_METHOD_CALL_NODE_HPP

#include <string>
#include <vector>
#include <llvm/IR/IRBuilder.h>
#include <context/AstContext.hpp>
#include "FunctionCallNode.hpp"
#include "ChainedDereferenceNode.hpp"


namespace LynxAst {
    
    using namespace LynxContext;
    using namespace LynxConstants;
    
    class MethodCallNode : public Node {

        public:

            /**
             * @struct MethodCallInfo
             * @brief Encapsulates all information needed to invoke a method.
             *
             * Contains references to the AST context, the target class type, the
             * LLVM value for the object instance, the method name (both readable
             * and mangled), and the list of LLVM argument values.
             */
            template <typename Adapter>
            struct MethodCallInfo {

                // Allow only InterfaceType, ClassType or MixinType
                static_assert(
                    std::is_same<Adapter, InterfaceType>::value ||
                    std::is_same<Adapter, ClassType>::value   ||
                    std::is_same<Adapter, MixinType>::value,
                    "MethodCallInfo can only be instantiated with InterfaceType, ClassType, or MixinType"
                );
            
                AstContext& astContext;             

                const Adapter& usrDefinedType; // can be interface type or class Type
                   
                llvm::Value* const instance;  // Pointer is fixed once set, but the underlying Value* can still be used in IR ops

                const std::string& methodName; // Read-only reference to method’s logical name

                const std::string& mangleName; // Read-only reference to mangled name

                const std::vector<llvm::Value*> argValues; // Read-only reference to arguments

                const std::vector<llvm::Type*> argTypes; // Read-only reference to argument types
            };


        private:
        
            std::unique_ptr<FunctionCallNode> functionCallNode;

            std::unique_ptr<Node> objectTargetNode;

            std::tuple<std::string, std::vector<llvm::Type*>, std::vector<llvm::Value*>> extractMethodCall(const AstContext& astContext);

            llvm::Value* createNullSafeCall(AstContext& astContext, llvm::Value* objectPtr, const std::function<llvm::Value*()>& generateCall);
            
            /**
             * @brief Handles method calls on built-in or library types.
             * @param astContext Shared pointer to AST context.
             * @param objectValue LLVM value representing the object.
             * @param baseType representing the result of datatype
             * @return LLVM value representing the result of the call.
            */
            llvm::Value* dispatchBuiltinOrLibraryMethod(std::shared_ptr<AstContext> astContext, const BaseType& baseType, llvm::Value* objectValue);

            /**
             * @brief Handles method calls on user-defined classes or interfaces.
             * @param astContext Reference to AST context.
             * @param objectValue LLVM value representing the object.
            * @param baseType representing the result of datatype
             * @return LLVM value representing the result of the call.
            */
            llvm::Value* dispatchUserDefinedMethod(AstContext& astContext, 
                const BaseType& baseType, 
                llvm::Value* objectValue,
                std::string funcName,
                std::vector<llvm::Type*> argTypes,
                std::vector<llvm::Value*> argValues
            );

            /**
             * @brief Classifies the target of the method call.
             *
             * Determines whether the target is a built-in/library type or a user-defined type.
             *
             * @param astContext Reference to the AST context.
             * @return Tuple containing:
             *         - LLVM value of the object
             *         - Type category as string ("File", "DateTime", "Array", "other")
             *         - Boolean indicating whether it's a user-defined type
             */
            std::tuple<llvm::Value*, BaseType*, bool> classifyMethodTarget(AstContext& astContext);

            /**
             * @brief Dispatches a virtual method call at runtime.
             *
             * Uses runtime structures (like vtables) to find and invoke the correct
             * method implementation.
             *
             * @param info MethodCallInfo containing context, target, and arguments
             * @return LLVM value of the method call result
             */
            template <typename Adapter>
            llvm::Value* dispatchMethod(const MethodCallInfo<Adapter>& info);

            /**
             * @brief Invokes a method, choosing compile-time or runtime dispatch.
             *
             * Determines the appropriate dispatch strategy and generates LLVM IR.
             *
             * @param info MethodCallInfo containing context, target, and arguments
             * @return LLVM value of the method call result
             */
            template <typename Adapter>
            llvm::Value* invokeMethod(const MethodCallInfo<Adapter>& info);

        public:

            MethodCallNode(
                std::unique_ptr<FunctionCallNode> fnCallNode, 
                std::unique_ptr<Node> targetNode
            ): functionCallNode(std::move(fnCallNode)), objectTargetNode(std::move(targetNode)) {}

            std::unique_ptr<Node> clone() const override;

            NodeType getNodeType() override { return NodeType::METHOD_CALL_NODE; }

            llvm::Value* generateCode(std::shared_ptr<AstContext> astContext) override;

            ~MethodCallNode() override = default;
    };

}

#endif
