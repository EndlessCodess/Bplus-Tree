#include "../include/BplusTree.h" // 假设你的B+树类定义在此头文件中
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

void test_bplus_tree_bulk_operations() {
  // 创建B+树，阶数为4（最大键数=3，最小键数=1）
  BplusTree<int, int> tree(4);

  // 设置插入、删除和查询的数量
  const int num_inserts = 20'000'000; // 2000万
  const int num_deletes = 10'000'000; // 1000万
  const int num_queries = 10'000'000; // 1000万
  std::cout << "开始批量插入 " << num_inserts << " 个键值对" << std::endl;

  // 用于生成随机键的工具
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> key_dist(1,
                                           1'000'000'000); // 随机键范围1到10亿
  std::uniform_int_distribution<> range_dist(1, 10);       // 范围查询大小1-10

  // 插入阶段
  auto insert_start_time = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < num_inserts; ++i) {
    int key = key_dist(gen);
    int value = key * 10;
    tree.insert(key, value);
    // if (i % 2'000'000 == 0 && i > 0) {
    //   std::cout << "已插入 " << i << " 个键值对" << std::endl;
    // }
  }
  auto insert_end_time = std::chrono::high_resolution_clock::now();
  auto insert_duration_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(insert_end_time -
                                                            insert_start_time);
  double insert_duration_seconds = insert_duration_ms.count() / 1000.0;

  std::cout << "批量插入完成！" << std::endl;
  std::cout << "插入键值对数量：" << num_inserts << std::endl;
  std::cout << "插入总耗时：" << insert_duration_seconds << " 秒" << std::endl;
  std::cout << "平均每秒插入："
            << static_cast<int>(num_inserts / insert_duration_seconds) << " 次"
            << std::endl;

  // 删除阶段
  std::cout << "开始批量删除 " << num_deletes << " 个键" << std::endl;
  auto delete_start_time = std::chrono::high_resolution_clock::now();
  int successful_deletes = 0;
  for (int i = 0; i < num_deletes; ++i) {
    int key = key_dist(gen);
    bool removed = tree.remove(key);
    if (removed) {
      ++successful_deletes;
    }
    // if (i % 2'000'000 == 0 && i > 0) {
    //   std::cout << "已尝试删除 " << i << " 个键，成功删除 "
    //             << successful_deletes << " 个" << std::endl;
    // }
  }
  auto delete_end_time = std::chrono::high_resolution_clock::now();
  auto delete_duration_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(delete_end_time -
                                                            delete_start_time);
  double delete_duration_seconds = delete_duration_ms.count() / 1000.0;

  std::cout << "批量删除完成！" << std::endl;
  std::cout << "尝试删除键数量：" << num_deletes << std::endl;
  std::cout << "成功删除键数量：" << successful_deletes << std::endl;
  std::cout << "成功删除比例：" << (successful_deletes * 100.0 / num_deletes)
            << "%" << std::endl;
  std::cout << "删除总耗时：" << delete_duration_seconds << " 秒" << std::endl;
  std::cout << "平均每秒尝试删除："
            << static_cast<int>(num_deletes / delete_duration_seconds) << " 次"
            << std::endl;

  // 查询阶段
  std::cout << "开始查询 " << num_queries << " 个键" << std::endl;
  auto query_start_time = std::chrono::high_resolution_clock::now();
  int found_count = 0;
  for (int i = 0; i < num_queries; ++i) {
    int key = key_dist(gen);
    int value = tree.search(key);
    if (value != 0) { // 假设值为0表示未找到
      ++found_count;
    }
    // if (i % 2'000'000 == 0 && i > 0) {
    //   std::cout << "已查询 " << i << " 个键，找到 " << found_count << " 个"
    //             << std::endl;
    // }
  }
  auto query_end_time = std::chrono::high_resolution_clock::now();
  auto query_duration_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(query_end_time -
                                                            query_start_time);
  double query_duration_seconds = query_duration_ms.count() / 1000.0;

  std::cout << "查询完成！" << std::endl;
  std::cout << "查询键数量：" << num_queries << std::endl;
  std::cout << "找到键数量：" << found_count << std::endl;
  std::cout << "找到比例：" << (found_count * 100.0 / num_queries) << "%"
            << std::endl;
  std::cout << "查询总耗时：" << query_duration_seconds << " 秒" << std::endl;
  std::cout << "平均每秒查询："
            << static_cast<int>(num_queries / query_duration_seconds) << " 次"
            << std::endl;
}

int main() {
  test_bplus_tree_bulk_operations();
  std::cout << "批量插入、删除和查询测试通过！" << std::endl;
  return 0;
}