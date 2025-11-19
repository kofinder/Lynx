
# set(LIB_MODULE_ANALYZER_STATE_SOURCES
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/ClassState.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/InterfaceState.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/MixinState.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/EnumState.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/FunctionState.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/VariableState.hpp

#     ${CMAKE_CURRENT_SOURCE_DIR}/state/impl/ClassState.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/impl/InterfaceState.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/impl/EnumState.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/impl/MixinState.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/impl/FunctionState.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/state/impl/VariableState.cpp
# )

# set(LIB_MODULE_ANALYZER_INTERFACE_SOURCES
#     ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/SemanticState.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/SemanticAnalyzer.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/SemanticThreadPool.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/SemanticStateFactory.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/impl/SemanticAnalyzer.cpp
# )

# add_library(LynxAnalyzer STATIC 
#     ${LIB_MODULE_ANALYZER_STATE_SOURCES} 
#     ${LIB_MODULE_ANALYZER_INTERFACE_SOURCES}
# )

# target_include_directories(LynxAnalyzer PUBLIC ${LIBRARY_INCLUDES})

# target_link_libraries(LynxAnalyzer PUBLIC 
#     LynxLogger 
#     LynxTypes 
#     LynxConstants 
#     LynxContext
#     LynxAst
# )

# =========================================
# Gather sources and headers
# =========================================
file(GLOB_RECURSE ANALYZER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/interfaces/impl/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/state/impl/*.cpp"
)

file(GLOB_RECURSE ANALYZER_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/interfaces/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/state/*.hpp"
)

# =========================================
# Create static library
# =========================================
add_library(LynxAnalyzer STATIC
    ${ANALYZER_SOURCES}
    ${ANALYZER_HEADERS}
)

# =========================================
# Include directories
# =========================================
target_include_directories(LynxAnalyzer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/state
    ${LIBRARY_INCLUDES} # add global dependencies if needed
)

# =========================================
# Optional: link to other modules
# =========================================
target_link_libraries(LynxAnalyzer PUBLIC
    LynxLogger
    LynxContext
    LynxConstants
    LynxAst
    LynxTypes
)

# =========================================
# Message for clarity
# =========================================
message(STATUS "✅ LynxAnalyzer module configured")
