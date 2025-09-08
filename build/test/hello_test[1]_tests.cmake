add_test( HelloTest.BasicAssertions /root/xy/c_plusprojects/Bplus-Tree/build/test/hello_test [==[--gtest_filter=HelloTest.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties( HelloTest.BasicAssertions PROPERTIES WORKING_DIRECTORY /root/xy/c_plusprojects/Bplus-Tree/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( hello_test_TESTS HelloTest.BasicAssertions)
