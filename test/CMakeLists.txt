add_executable(testWrite testWrite.cpp)
target_link_libraries(testWrite -lboost_unit_test_framework)
add_test(testWrite testWrite)
