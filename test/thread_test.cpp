#include "../include/BplusTree.h" // 假设你的B+树类定义在此头文件中
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

void concurrent_insert(BplusTree<int, int> &tree, int thread_id,
                       int num_inserts, std::random_device &rd,
                       std::mt19937 &gen,
                       std::uniform_int_distribution<> &key_dist,
                       int &progress) {
  (void)gen;
  std::mt19937 local_gen(rd());     // 每个线程有独立的随机数生成器
  local_gen.seed(rd() + thread_id); // 避免种子冲突
  for (int i = 0; i < num_inserts; ++i) {
    int key = key_dist(local_gen);
    int value = key * 10;
    tree.insert(key, value);                 // 依赖内部锁
    if (thread_id == 0 && i % 30'000 == 0) { // 只由主线程更新进度
      progress += 30'000;
      if (progress % 3'00'000 == 0) {
        std::cout << "已插入 " << progress << " 个键值对" << std::endl;
      }
    }
  }
}

void test_bplus_tree_concurrent_insert() {
  const int total_inserts = 3'000'000; // 总插入数量
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> key_dist(1, 1'000'000'000); // 随机键1到10亿

  // 打开文件以写入结果
  std::ofstream outFile("./concurrent_insert_performance.csv");
  if (!outFile) {
    std::cerr << "无法创建文件 concurrent_insert_performance.csv" << std::endl;
    return;
  }
  outFile << "Degree,MaxKeys,Threads,TotalTime(s),InsertionsPerSecond\n";

  const int degree = 4; // 固定度数为4
  std::cout << "\n测试度数: " << degree << " (maxKeys=" << degree - 1 << ")"
            << std::endl;

  // 测试 1 到 24 线程
  for (int num_threads = 1; num_threads <= 16; ++num_threads) {
    BplusTree<int, int> tree(degree); // 创建新树实例
    std::vector<std::thread> threads;
    int inserts_per_thread = total_inserts / num_threads;
    int progress = 0;

    auto start_time = std::chrono::high_resolution_clock::now();

    // 启动线程，使用 lambda 封装
    for (int i = 0; i < num_threads; ++i) {
      threads.emplace_back([=, &tree, &rd, &gen, &key_dist, &progress]() {
        concurrent_insert(tree, i, inserts_per_thread, rd, gen, key_dist,
                          progress);
      });
    }

    // 等待所有线程完成
    for (auto &t : threads) {
      t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    double duration_seconds = duration_ms.count() / 1000.0;
    int insertions_per_second =
        static_cast<int>(total_inserts / duration_seconds);

    std::cout << "线程数: " << num_threads << " 插入完成！" << std::endl;
    std::cout << "总耗时: " << duration_seconds << " 秒" << std::endl;
    std::cout << "平均每秒插入: " << insertions_per_second << " 次"
              << std::endl;

    outFile << degree << "," << (degree - 1) << "," << num_threads << ","
            << duration_seconds << "," << insertions_per_second << "\n";
  }

  outFile.close();
  std::cout << "结果已保存到 concurrent_insert_performance.csv" << std::endl;
}

int main() {
  test_bplus_tree_concurrent_insert();
  std::cout << "并发插入测试通过！" << std::endl;
  return 0;
}