set(LIB_MODULE_INDLUE_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/AnyRegCallingConvSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/CFGuardCheckSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/ColdCallingConvSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/CXXFastTLSSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/FastCallingConvSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/FastCallSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/GHCCallingConvSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/HiPECallingConvSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/NumberedCallingConvSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/PreserveAllSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/PreserveMostSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/PreserveNoneSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/StdCallSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/SwiftSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/SwiftTailSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/ThisCallSpec.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/includes/TailSpec.hpp 
)

set(LIB_MODULE_SPECIFIC_SOURCE 
  ${CMAKE_CURRENT_SOURCE_DIR}/IsVarArgSpec.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/AndSpecification.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/HasAttributeSpec.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CallingConventionInferer.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CallingConventionSpecification.hpp
)

add_library(LynxCallConv STATIC
  ${LIB_MODULE_INDLUE_SOURCES} 
  ${LIB_MODULE_SPECIFIC_SOURCE}
)

llvm_map_components_to_libnames(LLVM_LIBS
  core
  mcjit
  executionengine
  native
  nativecodegen
  orcjit
  interpreter
  support
  irreader
  codegen
  transformutils
  scalaropts
)

target_link_libraries(LynxCallConv PRIVATE 
  LynxLogger 
  LynxConstants
  ${LLVM_LIBS}
)

target_include_directories(LynxCallConv PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/includes
  ${CMAKE_BINARY_DIR}/configured_files/include
)



