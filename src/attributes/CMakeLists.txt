add_library(LynxFunctionAttr INTERFACE)

target_include_directories(LynxFunctionAttr INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}/handler
)

target_include_directories(LynxFunctionAttr INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}/handler
)

target_link_libraries(LynxFunctionAttr INTERFACE
  LynxLogger
  LLVMCore
  LLVMSupport
  LLVMIRReader
)

