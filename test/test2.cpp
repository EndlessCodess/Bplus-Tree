#include "../include/BplusTree.h"
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <vector>

int main() {

  // 1.测试插入

  // 设置 B+ 树，m=3 maxKeys = 2
  BplusTree<int, uint64_t> tree(4);

  // 获取当前时间作为默认种子
  unsigned int seed = static_cast<unsigned int>(time(nullptr));
  std::cout << "Using seed: " << seed << "\n";
  std::cout << "Enter a different seed (or press Enter for default): ";
  std::string seedInput;
  std::getline(std::cin, seedInput);
  if (!seedInput.empty()) {
    try {
      seed = std::stoi(seedInput);
      std::cout << "Using custom seed: " << seed << "\n";
    } catch (const std::exception &e) {
      std::cout << "Invalid seed, using default: " << seed << "\n";
    }
  }

  // 初始化随机数生成器
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> dis(1, 1000); // 随机键范围 1-100

  // 插入 15 个随机键

  std::vector<int> insertedKeys; // 记录已插入的键

  const int numKeys = 30;
  std::cout << "Inserting " << numKeys << " random keys:\n";
  for (int i = 0; i < numKeys; ++i) {
    int key = dis(gen);
    uint64_t value = static_cast<uint64_t>(key * 10); // 值 = 键 * 10
    std::cout << "Inserting key: " << key << ", value: " << value << "\n";
    tree.insert(key, value);
    insertedKeys.push_back(key);
  }

  // 输出树结构
  std::cout << "\nTree structure:\n";
  tree.printBplusTree(tree.getRoot(), 0);

  // 输出中序遍历
  std::cout << "Inorder traversal: ";
  tree.inorderTraversal();
  std::cout << "\n";

  // 输出树高度和节点数
  std::cout << "Height: " << tree.getTreeHeight(tree.getRoot()) << "\n";
  std::cout << "Node count: " << tree.countNode() << "\n";

  // 2.测试查找

  // 2.1单一查找

  // 随机的key
  // std::cout << "\nTesting search:\n";
  // for (int i = 0; i < numKeys; ++i) {
  //   int key = dis(gen);
  //   auto value = tree.search(key);
  //   if (value) {
  //     std::cout << "Found key: " << key << ", value: " << value << "\n";
  //   } else {
  //     std::cout << "Key " << key << " not found.\n";
  //   }
  // }

  // // 存在的key
  // std::cout << "\nTesting search:\n";
  // for (int i = 0; i < numKeys; ++i) {
  //   int key = insertedKeys[i];
  //   auto value = tree.search(key);
  //   if (value) {
  //     std::cout << "Found key: " << key << ", value: " << value << "\n";
  //   } else {
  //     std::cout << "Key " << key << " not found.\n";
  //   }
  // }

  // 2.2范围查找
  int rangeStart = 100;
  int rangeEnd = 300;
  std::cout << "\nRange search from " << rangeStart << " to " << rangeEnd
            << ":\n";
  auto rangeResults = tree.rangeSearch(rangeStart, rangeEnd);
  if (rangeResults.empty()) {
    std::cout << "No keys found in the range.\n";
  } else {
    for (const auto &result : rangeResults) {
      std::cout << "Key: " << result.first << ", Value: " << result.second
                << "\n";
    }
  }

  return 0;
}