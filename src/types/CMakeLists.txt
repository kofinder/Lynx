# ------------------------------------------------------------------------
#  Collect Source Files Recursively
# ------------------------------------------------------------------------
file(GLOB_RECURSE LYNX_TYPES_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*/impl/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

if(NOT LYNX_TYPES_SOURCES)
    message(FATAL_ERROR "No source files found in Lynx Types module!")
endif()

# ------------------------------------------------------------------------
#  Create the Static Library
# ------------------------------------------------------------------------
add_library(LynxTypes STATIC ${LYNX_TYPES_SOURCES})

# ------------------------------------------------------------------------
#  Include Directories
# ------------------------------------------------------------------------
target_include_directories(LynxTypes PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential
    ${CMAKE_CURRENT_SOURCE_DIR}/associative
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined
    ${CMAKE_CURRENT_SOURCE_DIR}/resolver
    ${CMAKE_CURRENT_SOURCE_DIR}/resolver/methods
    ${CMAKE_CURRENT_SOURCE_DIR}/resolver/strategies
    ${CMAKE_CURRENT_SOURCE_DIR}/infrerence
    ${CMAKE_CURRENT_SOURCE_DIR}/constants
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/visitor
    ${CMAKE_CURRENT_SOURCE_DIR}/tmpl
    ${CMAKE_CURRENT_SOURCE_DIR}/wrapper
)

target_link_libraries(LynxTypes PRIVATE
    LynxErrors
    LynxLogger 
    LynxConstants 
    LynxContext 
)

message(STATUS "[LynxTypes] Library built successfully.")
