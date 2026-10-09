add_executable(
    union_server_tests
    test_union_server.cpp
)

target_link_libraries(
    union_server_tests
    PRIVATE
        my_library
        GTest::gtest_main
        ws2_32
)

include(GoogleTest)

gtest_discover_tests(union_server_tests)