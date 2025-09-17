#include "../include/BplusTree.h" // 假设你的B+树类定义在此头文件中
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

// 测试B+树的并发性
void test_bplus_tree_concurrency() {
  // 创建B+树，阶数为4（最大键数=3，最小键数=1）
  BplusTree<int, int> tree(4);

  // 用于生成随机数的工具
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> key_dist(1, 100);  // 随机键范围1-100
  std::uniform_int_distribution<> sleep_dist(1, 10); // 随机睡眠时间1-10ms

  // 读线程函数：执行查找和范围查找
  auto read_thread_func = [&tree, &gen, &key_dist, &sleep_dist](int thread_id) {
    for (int i = 0; i < 10; ++i) {
      // 随机选择一个键进行查找
      int key = key_dist(gen);
      int value = tree.search(key);
      std::cout << "线程 " << thread_id << " 查找键 " << key << "，得到值 "
                << value << std::endl;

      // 随机选择范围查找
      int start_key = key_dist(gen);
      int end_key = start_key + key_dist(gen) % 10; // 范围大小最多10
      auto results = tree.rangeSearch(start_key, end_key);
      std::cout << "线程 " << thread_id << " 范围查找 [" << start_key << ", "
                << end_key << "] 返回 " << results.size() << " 个结果"
                << std::endl;

      // 模拟一些工作量
      std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dist(gen)));
    }
  };

  // 写线程函数：执行插入和删除
  auto write_thread_func = [&tree, &gen, &key_dist,
                            &sleep_dist](int thread_id) {
    for (int i = 0; i < 10; ++i) { // 写操作较少
      // 随机插入
      int key = key_dist(gen);
      int value = key * 10; // 简单映射：值 = 键 * 10
      tree.insert(key, value);
      std::cout << "线程 " << thread_id << " 插入键 " << key << "，值 " << value
                << std::endl;

      // 随机删除
      key = key_dist(gen);
      bool removed = tree.remove(key);
      std::cout << "线程 " << thread_id << " 删除键 " << key
                << "，成功: " << (removed ? "是" : "否") << std::endl;

      // 模拟一些工作量
      std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dist(gen)));
    }
  };

  // 验证函数：检查数据一致性
  auto verify_tree = [&tree]() {
    // 插入一些已知键值对
    tree.insert(1, 10);
    tree.insert(2, 20);
    tree.insert(3, 30);

    // 验证查找
    assert(tree.search(1) == 10 && "查找键1失败"); // [MODIFY] 修正键值
    assert(tree.search(2) == 20 && "查找键2失败");
    assert(tree.search(3) == 30 && "查找键3失败");

    // 验证范围查找
    auto results = tree.rangeSearch(1, 3); // [MODIFY] 修正范围
    assert(results.size() >= 3 && "范围查找失败");
    bool found_1 = false, found_2 = false, found_3 = false;
    for (const auto &pair : results) {
      if (pair.first == 1 && pair.second == 10)
        found_1 = true;
      if (pair.first == 2 && pair.second == 20)
        found_2 = true;
      if (pair.first == 3 && pair.second == 30)
        found_3 = true;
    }
    assert(found_1 && found_2 && found_3 && "范围查找内容不正确");

    // 删除并验证
    assert(tree.remove(2) && "删除键2失败");
    assert(tree.search(2) == 0 && "键2在删除后仍存在");

    std::cout << "验证通过！" << std::endl;
  };

  // 创建线程：5个读线程，2个写线程
  std::vector<std::thread> threads;
  for (int i = 0; i < 5; ++i) {
    threads.emplace_back(read_thread_func, i + 1);
  }
  for (int i = 0; i < 2; ++i) {
    threads.emplace_back(write_thread_func, i + 1);
  }

  // 等待所有线程完成
  for (auto &t : threads) {
    t.join();
  }

  // 运行验证
  verify_tree();

  // 打印最终树结构（用于调试）
  std::cout << "最终树结构：" << std::endl;
  tree.inorderTraversal();
}

int main() {
  test_bplus_tree_concurrency();
  std::cout << "所有测试通过！" << std::endl;
  return 0;
}