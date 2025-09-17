#include "../include/BplusTree.h" // 假设你的B+树类定义在此头文件中
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

void test_bplus_tree_bulk_insert_delete() {
  // 创建B+树，阶数为4（最大键数=3，最小键数=1）
  BplusTree<int, int> tree(4);

  // 设置插入和删除的键值对数量
  const int num_inserts = 20'000'000; // 2000万
  const int num_deletes = 10'000'000; // 1000万
  std::cout << "开始批量插入 " << num_inserts << " 个键值对" << std::endl;

  // 用于生成随机键的工具
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> key_dist(1,
                                           1'000'000'000); // 随机键范围1到10亿

  // 插入阶段
  // 记录开始时间
  auto insert_start_time = std::chrono::high_resolution_clock::now();

  // 批量插入
  std::vector<int> inserted_keys; // 保存部分插入的键用于验证
  inserted_keys.reserve(100);     // 验证100个随机键
  for (int i = 0; i < num_inserts; ++i) {
    int key = key_dist(gen);
    int value = key * 10; // 值 = 键 * 10
    tree.insert(key, value);
    if (i < 100) { // 保存前100个键用于验证
      inserted_keys.push_back(key);
    }
    if (i % 2'000'000 == 0 && i > 0) {
      std::cout << "已插入 " << i << " 个键值对" << std::endl;
    }
  }

  // 记录插入结束时间
  auto insert_end_time = std::chrono::high_resolution_clock::now();
  auto insert_duration_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(insert_end_time -
                                                            insert_start_time);
  double insert_duration_seconds = insert_duration_ms.count() / 1000.0;

  // 输出插入性能
  std::cout << "批量插入完成！" << std::endl;
  std::cout << "插入键值对数量：" << num_inserts << std::endl;
  std::cout << "插入总耗时：" << insert_duration_seconds << " 秒" << std::endl;
  std::cout << "平均每秒插入："
            << static_cast<int>(num_inserts / insert_duration_seconds) << " 次"
            << std::endl;

  // 验证插入正确性
  for (int key : inserted_keys) {
    int value = tree.search(key);
    (void)value;
    assert(value == key * 10 && "插入键查找失败");
  }
  std::cout << "插入验证通过：随机检查的键值对均正确" << std::endl;

  // 删除阶段
  std::cout << "开始批量删除 " << num_deletes << " 个键" << std::endl;

  // 记录删除开始时间
  auto delete_start_time = std::chrono::high_resolution_clock::now();

  // 批量删除
  std::vector<int> deleted_keys; // 保存部分删除的键用于验证
  deleted_keys.reserve(100);     // 验证100个随机删除的键
  int successful_deletes = 0;    // 统计成功删除的次数
  for (int i = 0; i < num_deletes; ++i) {
    int key = key_dist(gen);
    bool removed = tree.remove(key);
    if (removed) {
      ++successful_deletes;
      if (i < 100) { // 保存前100个成功删除的键
        deleted_keys.push_back(key);
      }
    }
    if (i % 2'000'000 == 0 && i > 0) {
      std::cout << "已尝试删除 " << i << " 个键，成功删除 "
                << successful_deletes << " 个" << std::endl;
    }
  }

  // 记录删除结束时间
  auto delete_end_time = std::chrono::high_resolution_clock::now();
  auto delete_duration_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(delete_end_time -
                                                            delete_start_time);
  double delete_duration_seconds = delete_duration_ms.count() / 1000.0;

  // 输出删除性能
  std::cout << "批量删除完成！" << std::endl;
  std::cout << "尝试删除键数量：" << num_deletes << std::endl;
  std::cout << "成功删除键数量：" << successful_deletes << std::endl;
  std::cout << "删除总耗时：" << delete_duration_seconds << " 秒" << std::endl;
  std::cout << "平均每秒尝试删除："
            << static_cast<int>(num_deletes / delete_duration_seconds) << " 次"
            << std::endl;

  // 验证删除正确性
  for (int key : deleted_keys) {
    int value = tree.search(key);
    (void)value;
    assert(value == 0 && "删除键后仍存在");
  }
  std::cout << "删除验证通过：随机检查的已删除键均不存在" << std::endl;

  // 打印最终树结构（仅部分输出，避免过多数据）
  // std::cout << "最终树结构（中序遍历，仅部分输出）：" << std::endl;
  // tree.inorderTraversal(); // 假设inorderTraversal已实现
}

int main() {
  test_bplus_tree_bulk_insert_delete();
  std::cout << "批量插入和删除测试通过！" << std::endl;
  return 0;
}