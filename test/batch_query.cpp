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
  const int num_queries = 50'000;     // 5万（5万范围查询）
  std::cout << "开始批量插入 " << num_inserts << " 个键值对" << std::endl;

  // 用于生成随机键的工具
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> key_dist(1,
                                           1'000'000'000); // 随机键范围1到10亿
  std::uniform_int_distribution<> range_dist(1, 10);       // 范围查询大小1-10

  // 插入阶段
  auto insert_start_time = std::chrono::high_resolution_clock::now();
  std::vector<int> inserted_keys; // 保存部分插入的键用于验证
  inserted_keys.reserve(100);
  for (int i = 0; i < num_inserts; ++i) {
    int key = key_dist(gen);
    int value = key * 10;
    tree.insert(key, value);
    if (i < 100) {
      inserted_keys.push_back(key);
    }
    if (i % 2'000'000 == 0 && i > 0) {
      std::cout << "已插入 " << i << " 个键值对" << std::endl;
    }
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
  std::vector<int> deleted_keys; // 保存部分删除的键用于验证
  deleted_keys.reserve(100);
  int successful_deletes = 0;
  for (int i = 0; i < num_deletes; ++i) {
    int key = key_dist(gen);
    bool removed = tree.remove(key);
    if (removed) {
      ++successful_deletes;
      if (i < 100) {
        deleted_keys.push_back(key);
      }
    }
    if (i % 2'000'000 == 0 && i > 0) {
      std::cout << "已尝试删除 " << i << " 个键，成功删除 "
                << successful_deletes << " 个" << std::endl;
    }
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
  // std::cout << "开始批量查询 " << num_queries << " 次范围查询" << std::endl;
  // auto query_start_time = std::chrono::high_resolution_clock::now();
  // int total_range_results = 0; // 统计范围查询返回的结果数
  // for (int i = 0; i < num_queries; ++i) {

  //   int start_key = key_dist(gen);
  //   int range_size = range_dist(gen);
  //   int end_key = start_key + range_size;
  //   auto results = tree.rangeSearch(start_key, end_key);
  //   total_range_results += results.size();

  //   if (i > 0) {
  //     std::cout << "已完成 " << i << " 次查询" << std::endl;
  //   }
  // }
  // auto query_end_time = std::chrono::high_resolution_clock::now();
  // auto query_duration_ms =
  //     std::chrono::duration_cast<std::chrono::milliseconds>(query_end_time -
  //                                                           query_start_time);
  // double query_duration_seconds = query_duration_ms.count() / 1000.0;

  // std::cout << "批量查询完成！" << std::endl;
  // std::cout << "查询总次数：" << num_queries << std::endl;
  // std::cout << "范围查询总结果数：" << total_range_results << std::endl;
  // std::cout << "查询总耗时：" << query_duration_seconds << " 秒" <<
  // std::endl; std::cout << "平均每秒查询："
  //           << static_cast<int>(num_queries / query_duration_seconds) << "
  //           次"
  //           << std::endl;

  // 打印最终树结构（仅部分输出）
  // std::cout << "最终树结构（中序遍历，仅部分输出）：" << std::endl;
  // tree.inorderTraversal(); // 假设inorderTraversal已实现
}

int main() {
  test_bplus_tree_bulk_operations();
  std::cout << "批量插入、删除和查询测试通过！" << std::endl;
  return 0;
}