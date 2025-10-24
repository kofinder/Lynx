# Specify the directories for library includes.
set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

# set(GENERATED_BISON_HEADERS
#     ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
#     ${CMAKE_CURRENT_BINARY_DIR}/location.hh
#     ${CMAKE_CURRENT_BINARY_DIR}/position.hh
# )


# Include current source and binary directories for header file searches.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Find and include Bison and Flex packages for parser and lexer generation.
find_package(BISON REQUIRED)   # Bison is a parser generator.
find_package(FLEX REQUIRED)    # Flex is a lexer generator.

# Define a Bison target for generating the parser.
# This creates parser.cpp and parser.hpp in the current binary directory.
bison_target(LynxParser lynx_syntax_analyzer.yy 
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp 
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
    COMPILE_FLAGS "--report=all --locations")

# Define a Flex target for generating the scanner.
# This creates scanner.cpp in the current binary directory.
flex_target(LynxScanner lynx_lexical_analyzer.lex 
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)

# Establish a dependency between the Flex and Bison targets.
# This ensures that the scanner is generated before the parser.
add_flex_bison_dependency(LynxScanner LynxParser)

# Set compiler properties for the generated scanner source file to ignore unused parameter warnings.
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp PROPERTIES COMPILE_FLAGS -Wno-unused-parameter)

# Create a static library named 'lang' which includes the outputs from Flex and Bison, 
# along with other necessary source and header files.
add_library(LynxLang
    STATIC
    ${FLEX_LynxScanner_OUTPUTS}   # Include generated scanner output.
    ${BISON_LynxParser_OUTPUTS}    # Include generated parser output.
    ${GENERATED_BISON_HEADERS}
    LynxScanner.hpp                # Include the scanner header.
    LynxDriver.hpp                 # Include the driver header.
    LynxTokenizer.hpp
    LexerState.hpp
    TypeNestingLimiter.hpp
    LiteralNestingLimiter.hpp
    impl/LynxDriver.cpp
)


# Generate grammar.json from Bison output
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/grammar.json
    DEPENDS ${BISON_LynxParser_OUTPUTS} ${FLEX_LynxScanner_OUTPUTS}
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/generate_vscode_json.py
            ${CMAKE_CURRENT_BINARY_DIR}/parser.output
            ${CMAKE_CURRENT_BINARY_DIR}/grammar.json
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Custom target to always generate grammar.json when needed
add_custom_target(generate_lynx_json ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/grammar.json)


# Link the 'lang' library with a logger library privately.
# This means that the logger library will only be visible to this library.
target_link_libraries(LynxLang PRIVATE 
    Boost::system 
    Boost::filesystem
    Boost::program_options
    LynxLogger 
    LynxConstants 
    LynxAst
)

# Optional: Print debug information to the console.
message(STATUS "FLEX_OUTPUTS: ${FLEX_LynxScanner_OUTPUTS}")  # Display Flex outputs for debugging.
message(STATUS "BISON_OUTPUTS: ${BISON_LynxParser_OUTPUTS}")   # Display Bison outputs for debugging.