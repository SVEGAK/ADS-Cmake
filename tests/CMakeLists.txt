create_executable_project(DS_TESTS)
target_link_libraries(DS_TESTS gtest gtest_main)
add_test(NAME DS_TESTS COMMAND DS_TESTS)