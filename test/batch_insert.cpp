#include "../include/BplusTree.h" // 假设你的B+树类定义在此头文件中
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

void test_bplus_tree_bulk_insert() {
  const int num_inserts = 10'000'000; // 500万
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> key_dist(1, 1'000'000'000); // 随机键1到10亿

  // 打开文件以写入结果
  std::ofstream outFile("./insert_performance.csv");
  if (!outFile) {
    std::cerr << "无法创建文件 insert_performance.csv" << std::endl;
    return;
  }
  // 写入CSV头
  outFile << "Degree,MaxKeys,TotalTime(s),InsertionsPerSecond\n";

  // 测试度数从4到100，每次增加2
  for (int degree = 4; degree <= 4; degree += 2) {
    BplusTree<int, int> tree(degree);
    std::cout << "\n测试度数: " << degree << " (maxKeys=" << degree - 1 << ")"
              << std::endl;

    // 记录开始时间
    auto start_time = std::chrono::high_resolution_clock::now();

    // 批量插入
    for (int i = 0; i < num_inserts; ++i) {
      int key = key_dist(gen);
      int value = key * 10;
      tree.insert(key, value);
      if (i % 10'000'00 == 0 && i > 0) {
        std::cout << "已插入 " << i << " 个键值对" << std::endl;
      }
    }

    // 记录结束时间
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    double duration_seconds = duration_ms.count() / 1000.0;
    int insertions_per_second =
        static_cast<int>(num_inserts / duration_seconds);

    // 输出到控制台
    std::cout << "插入完成！" << std::endl;
    std::cout << "插入键值对数量: " << num_inserts << std::endl;
    std::cout << "总耗时: " << duration_seconds << " 秒" << std::endl;
    std::cout << "平均每秒插入: " << insertions_per_second << " 次"
              << std::endl;

    // 写入文件
    outFile << degree << "," << (degree - 1) << "," << duration_seconds << ","
            << insertions_per_second << "\n";
  }

  outFile.close();
  std::cout << "结果已保存到 insert_performance.csv" << std::endl;
}

int main() {
  test_bplus_tree_bulk_insert();
  std::cout << "批量插入测试通过！" << std::endl;
  return 0;
}