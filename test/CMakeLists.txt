
# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加测试可执行文件
add_executable(
    hello_test
    ${PROJECT_SOURCE_DIR}/test/hello_test.cc
)

add_executable(
    Bplus_query_test
    ${PROJECT_SOURCE_DIR}/test/Bplus_query_test.cpp
    ${PROJECT_SOURCE_DIR}/lib/libBplusTree.a
)

# 链接测试可执行文件到 Google Test 库
target_link_libraries(
    hello_test
    GTest::gtest_main
)

target_link_libraries(
    Bplus_query_test
    GTest::gtest_main
)

# 使用测试发现功能
include(GoogleTest)
gtest_discover_tests(hello_test)
gtest_discover_tests(Bplus_query_test)
