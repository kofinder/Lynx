file(GLOB_RECURSE LIB_AST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/impl/*.cpp"
)

file(GLOB_RECURSE LIB_AST_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/helper/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/tmpl/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.hpp"
)

set(LIBRARY_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_BINARY_DIR}/configured_files/include"
)

add_library(LynxAst STATIC ${LIB_AST_SOURCES} ${LIB_AST_HEADERS})

target_link_libraries(LynxAst PRIVATE 
    LynxLogger
    LynxExceptions
    LynxTypes
    LynxSystem
    LynxConstants
    LynxContext
    LynxCallConv
    LynxMangler
    LynxRuntime
    Boost::system
    Boost::filesystem
)

target_include_directories(LynxAst PUBLIC ${LIBRARY_INCLUDES})

