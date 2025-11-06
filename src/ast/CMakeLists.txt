set(LIB_AST_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/Node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/IdentifierNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/LiteralNode.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/impl/FunctionNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ExpressionNode.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/impl/EnumValueExpressionNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ExpressionStatementNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/StatementNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/AssignmentExpressionNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ArrayInitializerNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/FunctionCallNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/StatementListNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/IfStatementNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/SystemStatementNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/SwitchStatementNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/UnaryExpressionNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/BinaryExpressionNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/TernaryExpressionNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ComparisonExpressionNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ExceptionHandlerNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ForLoopStatementNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/VariableDeclarationNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/VariableDereferenceNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/EnumDeclarationNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ClazzDeclarationNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/MixinDeclarationNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/InterfaceDeclarationNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ObjectCreationNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/MethodCallNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ArrayAccessNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ChainedDereferenceNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/SuperConstructorCallNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/PrimaryExpressionNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ClazzConstructorNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ImportStatementNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/QualifiedFunctionCallNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/QualifiedIdentifierNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/StaticMethodCallNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/CollectionInitializerNode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ClazzConstructorExpressionNode.cpp
)

set(LIB_AST_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Node.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IdentifierNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LiteralNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FunctionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExpressionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EnumValueExpressionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExpressionStatementNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StatementNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AssignmentExpressionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ArrayInitializerNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FunctionCallNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StatementListNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IfStatementNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemStatementNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SwitchStatementNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ExceptionHandlerNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ForLoopStatementNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UnaryExpressionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BinaryExpressionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TernaryExpressionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EnumDeclarationNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ComparisonExpressionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VariableDeclarationNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VariableDeclarationNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClazzDeclarationNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MixinDeclarationNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InterfaceDeclarationNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ObjectCreationNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MethodCallNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ArrayAccessNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MethodCallNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClazzConstructorNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ChainedDereferenceNode.hpp  
    ${CMAKE_CURRENT_SOURCE_DIR}/SuperConstructorCallNode.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimaryExpressionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImportStatementNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QualifiedFunctionCallNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QualifiedIdentifierNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StaticMethodCallNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CollectionInitializerNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ClazzConstructorExpressionNode.hpp  
)

set(LIB_AST_HELPER
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/EmptyNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/LiteralPairNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/LiteralListNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/LiteralMapNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/ValuePlaceholderNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/SwitchCaseBlockNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/SwitchLevelExpressionNode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helper/SwitchCaseExpressionNode.hpp
)

set(LIB_AST_TEMPLATES
    ${CMAKE_CURRENT_SOURCE_DIR}/tmpl/ManglerTemplate.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tmpl/LLVMFunctionTemplate.hpp
)

set(LIB_AST_UTILS
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/FileUtils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/ClassUtils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/DatetimeUtils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/LLVMFunctionUtils.hpp
)


set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library(LynxAst STATIC 
    ${LIB_AST_SOURCES} 
    ${LIB_AST_HEADERS} 
    ${LIB_AST_UTILS}
    ${LIB_AST_HELPER}
    ${LIB_AST_TEMPLATES}
)

target_link_libraries(LynxAst PRIVATE 
    LynxLogger 
    LynxExceptions 
    LynxTypes 
    LynxSystem
    LynxConstants 
    LynxContext 
    LynxCallConv
    LynxResolver
    LynxMangler
    LynxLibRuntime
    Boost::system 
    Boost::filesystem
)

target_include_directories(LynxAst PUBLIC ${LIBRARY_INCLUDES})

