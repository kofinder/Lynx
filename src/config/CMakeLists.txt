
set(LIB_MODULE_PROGRAM_OPTION_CONFIG_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ProgramOptionConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/ProgramSourceProcessor.cpp
)

set(LIB_MODULE_PROGRAM_OPTION_CONFIG_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/ProgramOptionConfig.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProgramSyntaxColor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProgramTerminalColor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProgramOptionConstant.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ProgramSourceProcessor.hpp
)

add_library(LynxProgramConfig STATIC ${LIB_MODULE_PROGRAM_OPTION_CONFIG_SOURCES} ${LIB_MODULE_PROGRAM_OPTION_CONFIG_HEADER})

target_include_directories(LynxProgramConfig PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(LynxProgramConfig PUBLIC 
    yaml-cpp
    spdlog::spdlog
    cxxopts::cxxopts 
    Boost::system 
    Boost::filesystem
    Boost::program_options
    LynxLogger 
    LynxConstants 
    LynxContext
    LynxAst
    LynxLang
)
