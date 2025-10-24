set(LIB_MODULE_HANDLER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/SafeDefaultsHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/AlwaysInlineHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoInlineHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoReturnHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/ReadOnlyHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/ReadNoneHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoCaptureHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoUnwindHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/SanitizeAddressHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/OptimizeForSizeHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/InlineHintHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/StackProtectorHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoInlineHintHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoDeadStripHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/IntrinsicHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoRecurseHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/SanitizeThreadHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/SanitizeMemoryHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/SanitizeHWAddressHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoInlineAsmHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/UWTableHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/ZeroExtendHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/SignExtendHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/ArgMemOnlyHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/ReturnsTwiceHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/StrictFPHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/SanitizeCoverageHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/AssumeSameAlignedHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoSanitizeMemoryHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoSanitizeThreadHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/NoSanitizeAddressHandler.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/handler/DereferenceableParamHandler.hpp
)

set(LIB_MODULE_INTERFACE_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/impl/FunctionAttributeBuilder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/impl/FunctionAttributeInferer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/FunctionAttributeBuilder.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces//FunctionAttributeInferer.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/interfaces//FunctionAttributeHandler.hpp
)

add_library(LynxFunctionAttr STATIC 
  ${LIB_MODULE_HANDLER_SOURCES}
  ${LIB_MODULE_INTERFACE_SOURCES}
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

target_link_libraries(LynxFunctionAttr PRIVATE LynxLogger ${LLVM_LIBS})

target_include_directories(LynxFunctionAttr PUBLIC ${LIBRARY_INCLUDES})



