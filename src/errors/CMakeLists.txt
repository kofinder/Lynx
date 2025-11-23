set(LIB_ERROR_COMMON_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/LynxError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ErrorVisitor.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/LoggingVisitor.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ErrorHelpers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LynxError.cpp
)

set(LIB_ERROR_CONCRETE_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/TypeError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/ContextError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/LinkError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/ParserError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/SymbolError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/SyntaxError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/CodeGenError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/RuntimeError.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/CompositeError.hpp
)


add_library(LynxErrors STATIC 
    ${LIB_ERROR_COMMON_SOURCES}
    ${LIB_ERROR_CONCRETE_SOURCES}
)

target_include_directories(LynxErrors PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

message(STATUS "✅ LynxErrors module configured successfully! ")
