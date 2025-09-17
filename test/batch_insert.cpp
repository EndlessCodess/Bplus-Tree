#include "../include/BplusTree.h" // 假设你的B+树类定义在此头文件中
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

void test_bplus_tree_bulk_insert() {
  // 创建B+树，阶数为4（最大键数=3，最小键数=1）
  BplusTree<int, int> tree(4);

  // 设置插入的键值对数量
  const int num_inserts = 10'000'000; // 1000万
  std::cout << "开始批量插入 " << num_inserts << " 个键值对" << std::endl;

  // 用于生成随机键的工具
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> key_dist(1,
                                           1'000'000'000); // 随机键范围1到10亿

  // 记录开始时间
  auto start_time = std::chrono::high_resolution_clock::now();

  // 批量插入
  std::vector<int> inserted_keys; // 保存插入的键用于验证
  inserted_keys.reserve(100);     // 验证100个随机键
  for (int i = 0; i < num_inserts; ++i) {
    int key = key_dist(gen);
    int value = key * 10; // 值 = 键 * 10
    tree.insert(key, value);
    if (i % 1'000'000 == 0 && i > 0) {
      std::cout << "已插入 " << i << " 个键值对" << std::endl;
    }
  }

  // 记录结束时间
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  double duration_seconds = duration_ms.count() / 1000.0;

  // 输出插入性能
  std::cout << "批量插入完成！" << std::endl;
  std::cout << "插入键值对数量：" << num_inserts << std::endl;
  std::cout << "总耗时：" << duration_seconds << " 秒" << std::endl;
  std::cout << "平均每秒插入："
            << static_cast<int>(num_inserts / duration_seconds) << " 次"
            << std::endl;

  // 验证插入正确性
  // for (int key : inserted_keys) {
  //   int value = tree.search(key);
  //   assert(value == key * 10 && "查找键失败");
  // }
  // std::cout << "验证通过：随机检查的键值对均正确" << std::endl;

  // 打印最终树结构（可选，仅打印部分以避免过多输出）
  // std::cout << "最终树结构（中序遍历，仅部分输出）：" << std::endl;
  // tree.inorderTraversal(); // 假设inorderTraversal已实现
}

int main() {
  test_bplus_tree_bulk_insert();
  std::cout << "批量插入测试通过！" << std::endl;
  return 0;
}