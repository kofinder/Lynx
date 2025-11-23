

set(LIB_MODULE_CODE_GEN_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/IRGenerator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/IRPlanner.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TopologicalSort.hpp
)

set(LIB_MODULE_CODE_GEN_HEADER 
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/IRGenerator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/IRPlanner.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/TopologicalSort.cpp
)

add_library(LynxCodegen STATIC ${LIB_MODULE_CODE_GEN_SOURCES} ${LIB_MODULE_CODE_GEN_HEADER})

target_link_libraries(LynxCodegen PRIVATE 
  LynxLogger 
  LynxAst 
  LynxLang
  LynxExceptions
  LynxProgramConfig
  Boost::system 
  Boost::filesystem
  Boost::program_options
)

target_include_directories(LynxCodegen PUBLIC ${LIBRARY_INCLUDES})

message(STATUS "✅ LynxCodegen module configured successfully! ")
