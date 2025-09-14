#include "../include/BplusTree.h"
#include <ctime>
#include <iostream>
#include <random>

int main() {
  // 设置 B+ 树，m=3 maxKeys = 2
  BplusTree<int, uint64_t> tree(6);

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
  const int numKeys = 30;
  std::cout << "Inserting " << numKeys << " random keys:\n";
  for (int i = 0; i < numKeys; ++i) {
    int key = dis(gen);
    uint64_t value = static_cast<uint64_t>(key * 10); // 值 = 键 * 10
    std::cout << "Inserting key: " << key << ", value: " << value << "\n";
    tree.insert(key, value);
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

  return 0;
}