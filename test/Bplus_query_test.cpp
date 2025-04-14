#include <gtest/gtest.h>
#include <BplusTree.h>

class BplusQueryTest : public ::testing::Test
{
protected:
    BplusTree<int, uint64_t> tree = BplusTree<int, uint64_t>(3);

    void SetUp() override
    {
        // 设置环境
        tree.insert(10, 100);
        tree.insert(20, 200);
        tree.insert(30, 300);
    }

    void TearDown() override
    {
        // 清理环境
    }
};

// 测试存在的key
TEST_F(BplusQueryTest, QueryExistingKey)
{
    EXPECT_EQ(100, tree.search(10));
    EXPECT_EQ(200, tree.search(20));
    EXPECT_EQ(300, tree.search(30));
}

// 测试不存在的key
TEST_F(BplusQueryTest, QueryNonExistingKey)
{
    EXPECT_EQ(uint64_t{}, tree.search(40));
}

// 测试边界条件
TEST_F(BplusQueryTest, QueryBoundaryKeys)
{
    EXPECT_EQ(uint64_t{}, tree.search(5));
}

// 主函数
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}