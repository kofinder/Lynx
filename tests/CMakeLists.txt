set(TEST_MAIN "unit_tests")
set(TEST_SOURCES "main.cc")
set(TEST_INCLUDEs "./")


add_executable(${TEST_MAIN} ${TEST_SOURCES})
target_link_libraries(${TEST_MAIN} PUBLIC "dslibs")
target_link_libraries(${TEST_MAIN} PRIVATE Catch2::Catch2WithMain)