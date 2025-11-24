
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
message(STATUS "✅ LynxAnalyzer module configured successfully!")
